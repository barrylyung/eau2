#pragma once
#include "object.h"
#include "column.h"
#include "data.h"
/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
 public:
  Schema schema_;
  // Array of Columns, each column of size 1
  union Data** elems_;
  size_t idx_;
  /** Build a row following a schema. */

  Row(Schema& scm){
    schema_ = scm;
    elems_ = new Data*[scm.width()];
  }

  void isCorrectType(size_t col, char data_type) {
    char col_type = schema_.col_type(col);
    if (col_type != data_type) {
      printf("Column type: %s does not match Schema Type: %s at index %zu\n",
            data_type, col_type, col);
      exit(1);
    }
  }

  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set(size_t col, int val){
    isCorrectType(col, 'I');
    elems_[col] = new Data;
    elems_[col]->i = val;
  }

  void set(size_t col, float val){
    isCorrectType(col, 'F');
    elems_[col] = new Data;
    elems_[col]->f = val;
  }
  void set(size_t col, bool val){
    isCorrectType(col, 'B');
    elems_[col] = new Data;
    elems_[col]->b = val;
  }
  /** The string is external. */
  void set(size_t col, String* val){
    isCorrectType(col, 'S');
    elems_[col] = new Data;
    elems_[col]->s = val;
  }

  /** Set/get the index of this row (ie. its position in the dataframe. This is
   *  only used for informational purposes, unused otherwise */
  void set_idx(size_t idx){
    idx_ = idx;
  }

  size_t get_idx(){
    return idx_;
  }

  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col){
    isCorrectType(col, 'I');
    return elems_[col]->i;
    //return elems_[col]->as_int()->get(0);
  }

  bool get_bool(size_t col){
    isCorrectType(col, 'B');
    return elems_[col]->b;
  }
  float get_float(size_t col){
    isCorrectType(col, 'F');
    return elems_[col]->f;
  }

  String* get_string(size_t col){
    isCorrectType(col, 'S');
    return elems_[col]->s;
  }


  /** Number of fields in the row. */
  size_t width(){
    return schema_.width();
  }

   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx){
    return schema_.col_type(idx);
  }

  /** Given a Fielder, visit every field of this row. The first argument is
    * index of the row in the dataframe.
    * Calling this method before the row's fields have been set is undefined. */
  void visit(size_t idx, Fielder& f) {
  f.start(idx);
  // Call fielder.accept for each item in array
  for (size_t i = 0; i < width(); i++) {
    switch (schema_.col_type(i)) {
      case 'I':
        f.accept(get_int(i));
        break;
      case 'B':
        f.accept(get_bool(i));
        break;
      case 'F':
        f.accept(get_float(i));
        break;
      case 'S':
        f.accept(get_string(i));
        break;
      default:
        myfail();
    }
  }
  f.done();
}

};
