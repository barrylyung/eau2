#pragma once
#include <stdio.h>
#include <string.h>
#include "string.h"
/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
   ArrOfStringArr* columnTypes_;
   ArrOfStringArr* columnNames_;
   ArrOfStringArr* rowNames_;

  /** Copying constructor */
  Schema(Schema& from) {
    columnTypes_ = from.columnTypes_->copy();
    columnNames_ = from.columnNames_->copy();
    rowNames_ = new ArrOfStringArr();
  }

  Schema(Schema& from, bool add_row_info): Schema(from) {
    delete rowNames_;
    if(add_row_info) {
          rowNames_ = from.rowNames_ -> copy();
    }
  }

  /** Create an empty schema **/
  Schema() {
    columnTypes_ = new ArrOfStringArr();
    columnNames_ = new ArrOfStringArr();
    rowNames_ = new ArrOfStringArr();
  }

  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) : Schema() {
    for(size_t i = 0; i < strlen(types); i++) {
      char type = types[i];
      if (type != 'S' && type != 'B' && type != 73 && type != 'F') {
        printf("Unsupported type\n");
        exit(1);
      }
      columnTypes_->pushBack(new String(&type));
      columnNames_->pushBack(new String(new char(i)));
    }
  }

  /** Add a column of the given type and name (can be nullptr), name
    * is external. Names are expectd to be unique, duplicates result
    * in undefined behavior. */
  void add_column(char typ, String* name) {
    columnTypes_->pushBack(new String(&typ));
    columnNames_->pushBack(name);
  }

  /** Add a row with a name (possibly nullptr), name is external.  Names are
   *  expectd to be unique, duplicates result in undefined behavior. */
  void add_row(String* name) {
    rowNames_->pushBack(name);
  }

  /** Return name of row at idx; nullptr indicates no name. An idx >= width
    * is undefined. */
  String* row_name(size_t idx) {
    return rowNames_->get(idx);
  }

  /** Return name of column at idx; nullptr indicates no name given.
    *  An idx >= width is undefined.*/
  String* col_name(size_t idx) {
    return columnNames_->get(idx);
  }

  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
    return columnTypes_->get(idx)->at(0);
  }

  /** Given a column name return its index, or -1. */
  int col_idx(const char* name) {
    for(size_t i = 0; i < columnNames_->size(); i++) {
      if (columnNames_->get(i)->equals(new String(name))) {
          return i;
      }
    }
    return -1;
  }

  /** Given a row name return its index, or -1. */
  int row_idx(const char* name) {
    for(size_t i = 0; i < rowNames_->size(); i++) {
      if (rowNames_->get(i)->equals(new String(name))) {
          return i;
      }
    }
    return -1;
  }

  /** The number of columns */
  size_t width() {
    return columnTypes_->size();
  }

  /** The number of rows */
  size_t length() {
    return rowNames_->size();
  }
};
