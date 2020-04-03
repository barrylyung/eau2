#pragma once
#include <stdlib.h>
#include "object.h"
#include "string.h"


class Array : public Object {
  public:
  	size_t size_;
  	size_t capacity_;

    Array() {
    };

    // Constructor
  	Array(size_t capacity) {
      size_ = 0;
  		capacity_ = capacity;
  	}

  	// Destructor
    ~Array() {
    }

  	// returns the length of the array
  	virtual size_t size() {
  		return this->size_;
  	}

  	// returns the number of elements currently in the array
  	virtual size_t capacity() {
  		return this->capacity_;
  	}

     void isOutOfBounds(size_t index) {
      if (index > this->size_ || index < 0) {
        printf("Index: %zu Out of Bounds for Array of size %zu", index, this->size_);
        exit(1);
      }
    }

    //virtual String* serialize() { return nullptr; }
};

class ObjArray : public Array {
public:
  Object** elems_;

  // Constructor
  ObjArray(size_t len) : Array(len) {
    elems_ = new Object* [len];
  }

  ~ObjArray() {};

  void insert(Object* obj, size_t index) {
      isOutOfBounds(index);
      elems_[index]  = obj;
  }

  void pushBack(Object* obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  Object* get(size_t index) {
    isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index, shifts rest of values down
  Object* remove(size_t index) {
    isOutOfBounds(index);
    Object* removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(Object* obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj -> equals(this->get(i))){
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    ObjArray* tempData = new ObjArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }

};

class IntArray : public Array {
public:
	int* elems_;

	IntArray(size_t len) : Array(len){
		elems_ = new int[len];
	}

	// inserts the given object into the array
	void insert(int obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index] = obj;
	}

  void pushBack(int obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

	// gets the object from data stored at index
  int get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

	// remove the item at the given index
	int remove(size_t index) {
    this->isOutOfBounds(index);
    int removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
	}

  size_t index_of(int obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    IntArray* tempData = new IntArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }
};

class BoolArray : public Array {
public:
	bool* elems_;

	BoolArray(size_t len) : Array(len){
		elems_ = new bool[len];
	}

// inserts the given object into the array
  void insert(bool obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index]  = obj;
  }

  void pushBack(bool obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  // gets the object from data stored at index
  bool get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index
  bool remove(size_t index) {
    this->isOutOfBounds(index);
    bool removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(bool obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    BoolArray* tempData = new BoolArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }
};

class FloatArray : public Array {
public:
  float* elems_;

	FloatArray(size_t len) : Array(len){
		elems_ = new float[len];
	}

  // inserts the given object into the array
  void insert(float obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index]  = obj;
  }

  void pushBack(float obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  // gets the object from data stored at index
  float get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index
  float remove(size_t index) {
    this->isOutOfBounds(index);
    float removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(float obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    FloatArray* tempData = new FloatArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }
};

class DoubleArray : public Array {
public:
  double* elems_;

  DoubleArray(size_t len) : Array(len){
		elems_ = new double[len];
	}

  // inserts the given object into the array
  void insert(double obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index]  = obj;
  }

  void pushBack(double obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  // gets the object from data stored at index
  double get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index
  double remove(size_t index) {
    this->isOutOfBounds(index);
    float removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(double obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    DoubleArray* tempData = new DoubleArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }

  String* serialize() {
    String* serial1= new String("{class: DoubleArray, size_: ");
    String* serial2 = new String(", capacity_: ");
    String* serial3 = new String(", elems_: Double[");
    String* serialClose = new String("]");
    String* classClose = new String("}");
    char temp[size_];
    snprintf(temp, sizeof(temp), "%zu", size_);
    char tempCap[capacity_];
    snprintf(tempCap, sizeof(tempCap), "%zu", capacity_);
    String* cap = new String(tempCap);
    String* size = new String(temp);
    for(size_t i = 0; i < size_; i ++) {
      char tempElem[sizeof(elems_[i])];
      snprintf(tempElem, sizeof(tempElem), "%f", elems_[i]);
      serial3 = serial3->strAppend(1, new String(tempElem));
      if(i != size_ - 1) {
        serial3 = serial3->strAppend(1, new String(", "));
      }
    }
    String* append = serial1->strAppend(6, size, serial2, cap, serial3, serialClose, classClose);
    return append;
  }
};

class SizeTArray : public Array {
public:
  size_t* elems_;

  SizeTArray(size_t len) : Array(len){
		elems_ = new size_t[len];
	}

  // inserts the given object into the array
  void insert(size_t obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index]  = obj;
  }

  void pushBack(size_t obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  // gets the object from data stored at index
  size_t get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index
  size_t remove(size_t index) {
    this->isOutOfBounds(index);
    float removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(size_t obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    SizeTArray* tempData = new SizeTArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }

  String* serialize() {
    String* serial1= new String("{class: SizeTArray, size_: ");
    String* serial2 = new String(", capacity_: ");
    String* serial3 = new String(", elems_: size_t[");
    String* serialClose = new String("]");
    String* classClose = new String("}");
    char temp[size_];
    snprintf(temp, sizeof(temp), "%zu", size_);
    char tempCap[capacity_];
    snprintf(tempCap, sizeof(tempCap), "%zu", capacity_);
    String* cap = new String(tempCap);
    String* size = new String(temp);
    for(size_t i = 0; i < size_; i ++) {
      char tempElem[sizeof(elems_[i])];
      snprintf(tempElem, sizeof(tempElem), "%zu", elems_[i]);
      serial3 = serial3->strAppend(1, new String(tempElem));
      if(i != size_ - 1) {
        serial3 = serial3->strAppend(1, new String(", "));
      }
    }
    String* append = serial1->strAppend(6, size, serial2, cap, serial3, serialClose, classClose);
    return append;
  }
};




class StringArray : public Array {
public:
  String** elems_;

  // Constructor
  StringArray(size_t len) : Array(len) {
    elems_ = new String* [len];
  }
  // inserts the given object into the array
  void insert(String* obj, size_t index) {
    this->isOutOfBounds(index);
    elems_[index]  = obj;
  }

  void pushBack(String* obj) {
      size_t tempSize = this->size();
      if (tempSize < this->capacity()) {
        this->insert(obj, tempSize);
      } else {
        grow();
        this->insert(obj, tempSize);
      }
      size_++;
  }

  // gets the object from data stored at index
  String* get(size_t index) {
    this->isOutOfBounds(index);
    return elems_[index];
  }

  // remove the item at the given index
  String* remove(size_t index) {
    this->isOutOfBounds(index);
    String* removed = get(index);
    for(size_t i = index; i < this->capacity(); i++) {
      this->insert(this->get(i+1), i);
    }
    this->size_--;
    return removed;
  }

  size_t index_of(String* obj) { // Returns the index of the first occurrence of o, or >size() if not there
    for(size_t i = 0; i < capacity_; i++) {
      if (this->get(i)){
        if(obj == this->get(i)) {
          return i;
        }
      }
    }
    return this->size() + 1;
  }

  void grow(){
    StringArray* tempData = new StringArray(capacity_ * 2);
    for(size_t i = 0; i < capacity_; i++) {
      tempData->pushBack(this->get(i));
    }
    this->elems_ = tempData->elems_;
    this->capacity_ = tempData->capacity_;
    this->size_ = tempData->size_;
  }

  String* serialize() {
    String* serial1= new String("{class: StringArray, size_: ");
    String* serial2 = new String(", capacity_: ");
    String* serial3 = new String(", elems_=String[");
    //String* serialElems = new String("\n");
    String* serialClose = new String("]");
    String* classClose = new String("}");
    char temp[size_];
    snprintf(temp, sizeof(temp), "%zu", size_);
    char tempCap[capacity_];
    snprintf(tempCap, sizeof(tempCap), "%zu", capacity_);
    String* cap = new String(tempCap);
    String* size = new String(temp);
    for(size_t i = 0; i < size_; i ++) {
      serial3 = serial3->strAppend(1, elems_[i]->serialize());
      if(i != size_ - 1) {
        serial3 = serial3->strAppend(1, new String(", "));
      }
    }
    String* append = serial1->strAppend(6, size, serial2, cap, serial3, serialClose, classClose);
    return append;
  }
};

class ArrOfIntArray: public Array {
public:
  IntArray** elems_;

  //Defaulting sizing to 100 for now, can change if need to scale larger
  ArrOfIntArray(): Array(100) {
    elems_ = new IntArray*[1];
    elems_[0] = new IntArray(100);
  }

  ~ArrOfIntArray() {};

  //Returns the int at the index
  int get(size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    return elems_[arrIdx]->get(elemIdx);
  }


  void insert(int elem, size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    if (elemIdx < elems_[arrIdx]->size()) {
      elems_[arrIdx]->insert(elem, elemIdx);
    } else {
      elems_[arrIdx]->pushBack(elem);
    }
  }

  void pushBack(int elem) {
    if (this->size() < this->capacity()) {
      this->insert(elem, this->size());
    } else {
        this->grow();
        insert(elem, this->size_);
      }
      size_++;
    }
  void grow() {
    size_t arrCount = this->capacity() / 100;
    IntArray** tempArr = new IntArray*[arrCount *2];

    for(size_t i = 0; i < arrCount; i++) {
      tempArr[i] = elems_[i];
    }
    for(size_t i = arrCount; i < arrCount * 2; i++) {
      tempArr[i] = new IntArray(100);
    }
    this->elems_ = tempArr;
    this->capacity_ = this->capacity_ * 2;
  }
};

class ArrOfFloatArray: public Array {
public:
  FloatArray** elems_;

  //Defaulting sizing to 100 for now, can change if need to scale larger
  ArrOfFloatArray(): Array(100) {
    elems_ = new FloatArray*[1];
    elems_[0] = new FloatArray(100);
  }

  ~ArrOfFloatArray() {};

  //Returns the int at the index
  float get(size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    return elems_[arrIdx]->get(elemIdx);
  }


  void insert(float elem, size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    if (elemIdx < elems_[arrIdx]->size()) {
      elems_[arrIdx]->insert(elem, elemIdx);
    } else {
      elems_[arrIdx]->pushBack(elem);
    }
  }

  void pushBack(float elem) {
    if (this->size() < this->capacity()) {
      this->insert(elem, this->size());
    } else {
        this->grow();
        insert(elem, this->size_);
      }
      size_++;
  }

  void grow() {
    size_t arrCount = this->capacity() / 100;
    FloatArray** tempArr = new FloatArray*[arrCount *2];

    for(size_t i = 0; i < arrCount; i++) {
      tempArr[i] = elems_[i];
    }
    for(size_t i = arrCount; i < arrCount * 2; i++) {
      tempArr[i] = new FloatArray(100);
    }
    this->elems_ = tempArr;
    this->capacity_ = this->capacity_ * 2;
  }
};

class ArrOfBoolArray: public Array {
public:
  BoolArray** elems_;

  //Defaulting sizing to 100 for now, can change if need to scale larger
  ArrOfBoolArray(): Array(100) {
    elems_ = new BoolArray*[1];
    elems_[0] = new BoolArray(100);
  }

  ~ArrOfBoolArray() {};

  //Returns the int at the index
  bool get(size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    return elems_[arrIdx]->get(elemIdx);
  }


  void insert(bool elem, size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    if (elemIdx < elems_[arrIdx]->size()) {
      elems_[arrIdx]->insert(elem, elemIdx);
    } else {
      elems_[arrIdx]->pushBack(elem);
    }
  }

  void pushBack(bool elem) {
    if (this->size() < this->capacity()) {
      this->insert(elem, this->size());
    } else {
        this->grow();
        insert(elem, this->size_);
      }
      size_++;
  }

  void grow() {
    size_t arrCount = this->capacity() / 100;
    BoolArray** tempArr = new BoolArray*[arrCount *2];

    for(size_t i = 0; i < arrCount; i++) {
      tempArr[i] = elems_[i];
    }
    for(size_t i = arrCount; i < arrCount * 2; i++) {
      tempArr[i] = new BoolArray(100);
    }
    this->elems_ = tempArr;
    this->capacity_ = this->capacity_ * 2;
  }
};

class ArrOfStringArr: public Array {
public:
  StringArray** elems_;

  //Defaulting sizing to 100 for now, can change if need to scale larger
  ArrOfStringArr(): Array(100) {
    elems_ = new StringArray*[1];
    elems_[0] = new StringArray(100);
  }

  ~ArrOfStringArr() {};

  //Returns the int at the index
  String* get(size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    return elems_[arrIdx]->get(elemIdx);
  }


  void insert(String* elem, size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    if (elemIdx < elems_[arrIdx]->size()) {
      elems_[arrIdx]->insert(elem, elemIdx);
    } else {
      elems_[arrIdx]->pushBack(elem);
    }
  }

  void pushBack(String* elem) {
    if (this->size() < this->capacity()) {
      this->insert(elem, this->size());
    } else {
        this->grow();
        insert(elem, this->size_);
      }
      size_++;
  }

  void grow() {
    size_t arrCount = this->capacity() / 100;
    StringArray** tempArr = new StringArray*[arrCount *2];

    for(size_t i = 0; i < arrCount; i++) {
      tempArr[i] = elems_[i];
    }
    for(size_t i = arrCount; i < arrCount * 2; i++) {
      tempArr[i] = new StringArray(100);
    }
    this->elems_ = tempArr;
    this->capacity_ = this->capacity_ * 2;
  }

  ArrOfStringArr* copy() {
    ArrOfStringArr* s = new ArrOfStringArr();
    for (size_t i = 0; i < size(); i++) {
      s -> pushBack(get(i));
    }
    return s;
  }
};


class ArrOfObjArr: public Array {
public:
  ObjArray** elems_;

  //Defaulting sizing to 100 for now, can change if need to scale larger
  ArrOfObjArr(): Array(100) {
    elems_ = new ObjArray*[1];
    elems_[0] = new ObjArray(100);
  }

  ~ArrOfObjArr() {};

  //Returns the int at the index
  Object* get(size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    return elems_[arrIdx]->get(elemIdx);
  }


  void insert(Object* elem, size_t index) {
    this->isOutOfBounds(index);
    size_t arrIdx = index / 100;
    size_t elemIdx = index % 100;
    if (elemIdx < elems_[arrIdx]->size()) {
      elems_[arrIdx]->insert(elem, elemIdx);
    } else {
      elems_[arrIdx]->pushBack(elem);
    }
  }

  void pushBack(Object* elem) {
    if (this->size() < this->capacity()) {
      this->insert(elem, this->size());
    } else {
        this->grow();
        insert(elem, this->size_);
      }
      size_++;
  }

  void grow() {
    size_t arrCount = this->capacity() / 100;
    ObjArray** tempArr = new ObjArray*[arrCount *2];

    for(size_t i = 0; i < arrCount; i++) {
      tempArr[i] = elems_[i];
    }
    for(size_t i = arrCount; i < arrCount * 2; i++) {
      tempArr[i] = new ObjArray(100);
    }
    this->elems_ = tempArr;
    this->capacity_ = this->capacity_ * 2;
  }

  ArrOfObjArr* copy() {
    ArrOfObjArr* s = new ArrOfObjArr();
    for (size_t i = 0; i < size(); i++) {
      s -> pushBack(get(i));
    }
    return s;
  }
};
