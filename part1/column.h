#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <cmath>
#include "object.h"
#include "string.h"
#include "util.h"
#include "array.h"


/**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;


class Column : public Object {
 public:
  char type_;

  /** Type converters: Return same column under its actual type, or
   *  nullptr if of the wrong type.  */
  virtual IntColumn* as_int() {}
  virtual BoolColumn*  as_bool() {}
  virtual FloatColumn* as_float() {}
  virtual StringColumn* as_string() {}

  /** Type appropriate push_back methods. Calling the wrong method is
    * undefined behavior. **/
  virtual void push_back(int val) {
  }

  virtual void push_back(bool val) {

  }
  virtual void push_back(float val) {

  }
  virtual void push_back(String* val) {

  }

  virtual size_t size() {}

  /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
  char get_type() {
    return this -> type_;
  }

};

/*************************************************************************
 * IntColumn::
 * Holds int values.
 */
class IntColumn : public Column {
 public:
  ArrOfIntArray* elems_;

  IntColumn() {
    elems_ = new ArrOfIntArray();
    type_ = 'I';
  }

  IntColumn(int n, ...) {
    //Start to read the variable list
    va_list v1;
    va_start(v1,n);
    elems_ = dynamic_cast<ArrOfIntArray*>(Util::buildArray(INT, n, v1));
  }

  int get(size_t idx) {
    return elems_->get(idx);
  }

  IntColumn* as_int() {
    return this;
  }

  /** Set value at idx. An out of bound idx is undefined.  */
  void set(size_t idx, int val) {
    elems_->insert(val, idx);
  }
  size_t size() {
    return elems_->size();
  }

  void push_back(int val) {
    elems_->pushBack(val);
  }
};

/*************************************************************************
 * BoolColumn::
 * Holds bool values.
 */
 class BoolColumn : public Column {
  public:
   ArrOfBoolArray* elems_;

   BoolColumn() {
     elems_ = new ArrOfBoolArray();
     type_ = 'B';
   }

   BoolColumn(int n, ...) {
     //Start to read the variable list
     va_list v1;
     va_start(v1,n);
     elems_ = dynamic_cast<ArrOfBoolArray*>(Util::buildArray(BOOL, n, v1));
   }

   bool get(size_t idx) {
     return elems_->get(idx);
   }

   BoolColumn* as_bool() {
     return this;
   }

   /** Set value at idx. An out of bound idx is undefined.  */
   void set(size_t idx, bool val) {
     elems_->insert(val, idx);
   }

   size_t size() {
     return elems_->size();
   }

   void push_back(bool val) {
     elems_->pushBack(val);
   }
 };

/*************************************************************************
 * FloatColumn::
 * Holds float values.
 */
 class FloatColumn : public Column {
  public:
   ArrOfFloatArray* elems_;

   FloatColumn() {
     elems_ = new ArrOfFloatArray();
     type_ = 'F';
   }

   FloatColumn(int n, ...) {
     //Start to read the variable list
     va_list v1;
     va_start(v1,n);
     elems_ = dynamic_cast<ArrOfFloatArray*>(Util::buildArray(FLOAT, n, v1));
   }

   float get(size_t idx) {
     return elems_->get(idx);
   }

   FloatColumn* as_float() {
     return this;
   }

   /** Set value at idx. An out of bound idx is undefined.  */
   void set(size_t idx, float val) {
     elems_->insert(val, idx);
   }

   size_t size() {
     return elems_->size();
   }

   void push_back(float val) {
     elems_->pushBack(val);
   }
 };

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
 class StringColumn : public Column {
  public:
   ArrOfStringArr* elems_;

   StringColumn() {
     elems_ = new ArrOfStringArr();
     type_ = 'S';
   }

   StringColumn(int n, ...) {
     //Start to read the variable list
     va_list v1;
     va_start(v1,n);
     elems_ = dynamic_cast<ArrOfStringArr*>(Util::buildArray(STRING, n, v1));
   }

   String* get(size_t idx) {
     return dynamic_cast<String*>(elems_->get(idx));
   }

   StringColumn* as_string() {
     return this;
   }

   /** Set value at idx. An out of bound idx is undefined.  */
   void set(size_t idx, String* val) {
     elems_->insert(val, idx);
   }

   size_t size() {
     return elems_->size();
   }

   void push_back(String* val) {
     elems_->pushBack(val);
   }
 };
