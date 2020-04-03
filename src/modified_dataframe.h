#pragma once
#include "object.h"
#include "column.h"
#include "schema.h"
#include "fielder.h"
#include "row.h"
#include "rower.h"
#include <thread>

#pragma once

#define NUM_THREADS 12

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class ModifiedDataFrame : public Object {
 public:
  Schema* schema_;
  ArrOfObjArr* cols_;
  size_t alloc_cols_;

  /** Create a data frame with the same columns as the given df but with no rows
   * or rownmaes */
  ModifiedDataFrame(ModifiedDataFrame& df) : ModifiedDataFrame(df.get_schema()){
    // Allocate enough columns to match this schema
    for (int i = 0; i < df.get_schema().length(); i++) {
        Row* r = new Row(*schema_);
        df.fill_row(i, *r);
        add_row(*r);
        delete r;
      }
  }

  /** Create a data frame from a schema and columns. All columns are created
   * empty. */
  ModifiedDataFrame(Schema& schema) {
    // Allocate enough columns to match this schema
    this->schema_ = new Schema();
    cols_ = new ArrOfObjArr();
    for (int i = 0; i < schema.width(); i++) {
        add_column(make_column(schema.col_type(i)), nullptr);
}
  }

  Column* make_column(char type) {
    if (type == 'I') {
        return new IntColumn();
    }
    else if (type == 'B') {
        return new BoolColumn();
    }
    else if (type == 'F') {
        return new FloatColumn();
    }
    else if (type == 'S') {
        return new StringColumn();
    }
    else {
        printf("Invalid type.\n");
        exit(1);
    }
  }

  ~ModifiedDataFrame() {
  }

  /** Returns the dataframe's schema. Modifying the schema after a dataframe
   * has been created in undefined. */
  Schema& get_schema() { return *schema_; }

  /** Adds a column this dataframe, updates the schema, the new column
   * is external, and appears as the last column of the dataframe, the
   * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column* col, String* name) {
    if (schema_->length() != col->size()) {
      printf("out of index");
      exit(1);
    }
    schema_->add_column(col->get_type(), name);
    cols_->pushBack(col);
  }

  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  int get_int(size_t col, size_t row) { return get_col(col)->as_int()->get(row); }
  bool get_bool(size_t col, size_t row) {
    return get_col(col)->as_bool()->get(row);
  }
  float get_float(size_t col, size_t row) {
    return get_col(col)->as_float()->get(row);
  }
  String* get_string(size_t col, size_t row) {
    return get_col(col)->as_string()->get(row);
  }

  /** Return the offset of the given column name or -1 if no such col. */
  int get_col(String& col) { return schema_->col_idx(col.c_str()); }

  /** Return the offset of the given row name or -1 if no such row. */
  int get_row(String& col) { return schema_->row_idx(col.c_str()); }

  Column* get_col(int i) {
    Object* elem = cols_->get(i);
    return dynamic_cast<Column*>(elem);
  }

  /** Set the value at the given column and row to the given value.
   * If the column is not  of the right type or the indices are out of
   * bound, the result is undefined. */
  void set(size_t col, size_t row, int val) {
     get_col(col)->as_int()->set(row, val);
  }
  void set(size_t col, size_t row, bool val) {
     get_col(col)->as_bool()->set(row, val);
  }
  void set(size_t col, size_t row, float val) {
     get_col(col)->as_float()->set(row, val);
  }
  void set(size_t col, size_t row, String* val) {
     get_col(col)->as_string()->set(row, val);
  }

  /** Set the fields of the given row object with values from the columns at
   * the given offset.  If the row is not form the same schema as the
   * dataframe, results are undefined.
   */
  void fill_row(size_t idx, Row& row) {
    for (size_t i = 0; i < ncols(); i++) {
      switch (schema_->col_type(i)) {
        case 'I':
          row.set(i, get_int(i, idx));
          break;
        case 'B':
          row.set(i, get_bool(i, idx));
          break;
        case 'F':
          row.set(i, get_float(i, idx));
          break;
        case 'S':
          row.set(i, get_string(i, idx));
          break;
        default:
          myfail();
      }
    }
  }

  /** Add a row at the end of this dataframe. The row is expected to have
   *  the right schema and be filled with values, otherwise undedined.  */
  void add_row(Row& row) {
    for (size_t i = 0; i < schema_->width(); i++) {
      switch (schema_->col_type(i)) {
        case 'I':
           get_col(i)->as_int()->push_back(row.get_int(i));
          break;
        case 'B':
          get_col(i)->as_bool()->push_back(row.get_bool(i));
          break;
        case 'F':
          get_col(i)->as_float()->push_back(row.get_float(i));
          break;
        case 'S':
          get_col(i)->as_string()->push_back(row.get_string(i));
          break;
        default:
          myfail();
      }
    }
    schema_->add_row(new String(new char(nrows())));
  }

  /** The number of rows in the dataframe. */
  size_t nrows() {
    if (schema_->width() == 0) return 0;
    return schema_->length();
  }

  /** The number of columns in the dataframe.*/
  size_t ncols() { return schema_->width(); }

  /** This method clones the Rower and executes the map in parallel. Join is
    * used at the end to merge the results. */
  void pmap(Rower& r) {
    size_t total_rows = nrows();
    if (total_rows < NUM_THREADS) {
      // just run sequentially in this case as it is likely faster.
      map(r);
      return;
    }
    size_t start_idx = 0;
    size_t increment = total_rows / NUM_THREADS;
    size_t end_idx = increment + (total_rows % NUM_THREADS);

    Rower** rowers = new Rower*[NUM_THREADS];
    std::thread threads[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++) {
      rowers[i] = static_cast<Rower*>(r.clone());

      threads[i] = std::thread(&ModifiedDataFrame::partial_map_, this, std::ref(*rowers[i]), start_idx, end_idx);

      start_idx = end_idx;
      end_idx += increment;
    }
    for (size_t i = 0; i < NUM_THREADS; i++) {
      threads[i].join();
      r.join_delete(rowers[i]);
    }

    delete[] rowers;
  }

  /** Visit rows in order from given start index (inclusive) to given end index (exclusive) */
  void partial_map_(Rower& r, size_t start_idx, size_t end_idx) {
    // Allocate one row to be reused
    Row* row = new Row(*schema_);
    for (size_t i = start_idx; i < end_idx; i++) {
      // Fill row, let rower mute row
      fill_row(i, *row);
      r.accept(*row);

      // Copy new row values back into dataframe
      for (size_t j = 0; j < schema_->width(); j++) {
        switch (schema_->col_type(j)) {
          case 'I':
            set(j, i, row->get_int(j));
                break;
          case 'B':
            set(j, i, row->get_bool(j));
                break;
          case 'F':
            set(j, i, row->get_float(j));
                break;
          case 'S':
            set(j, i, row->get_string(j));
                break;
          default:
            myfail();
        }
      }
    }
  }

  /** Visit rows in order */
  void map(Rower& r) {
    partial_map_(r, 0, nrows());
  }

  /** Create a new dataframe, constructed from rows for which the given Rower
   * returned true from its accept method. */
  ModifiedDataFrame* filter(Rower& r) {
    // Allocate new dataframe and row
    ModifiedDataFrame* df = new ModifiedDataFrame(*schema_);
    Row* row = new Row(*schema_);
    for (size_t i = 0; i < nrows(); i++) {
      fill_row(i, *row);
      if (r.accept(*row)) df->add_row(*row);
    }
    return df;
  }

  /** Print the dataframe in SoR format to standard output. */
  void print() {
    // Use a PrintRower to print all fields
    PrintRower p;
    Row* row = new Row(*schema_);
    for (size_t i = 0; i < nrows(); i++) {
      fill_row(i, *row);
      p.accept(*row);
    }
  }

};
