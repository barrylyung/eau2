#include "string.h"
/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
 public:
  virtual ~Fielder() {}

  /** Called before visiting a row, the argument is the row offset in the
    dataframe. */
  virtual void start(size_t r) {}

  /** Called for fields of the argument's type with the value of the field. */
  virtual void accept(bool b) {}
  virtual void accept(float f) {}
  virtual void accept(int i) {}
  virtual void accept(String* s) {}

  /** Called when all fields have been seen. */
  virtual void done() {}
};

/**
 * Fielder responsible for printing a row. When done, it begins the next line
 * authors: hughes.ni@husky.neu.edu, schoenberger.a@husky.neu.edu
 */
class PrintFielder : public Fielder {
 public:
  PrintFielder() {
  }
  ~PrintFielder() {}

  /** Called before visiting a row, the argument is the row offset in the
    dataframe. */
  void start(size_t r){};

  /** Called for fields of the argument's type with the value of the field. */
  void accept(bool b) { printf("%d,", b); }
  void accept(float f) { printf("%f,", f); }
  void accept(int i) { printf("%d,", i); }
  void accept(String* s) {
    printf("\"%s\",", s->c_str());}


  /** Called when all fields have been seen. */
  virtual void done() {
    printf("\n");
  }
};

class ValStringsFielder : public Fielder {
public:
    long total;

    ValStringsFielder() { total = 0; }
    ~ValStringsFielder() { }

    void start(size_t r) { }

    void accept(String* s) {
      for(int i = 0; i < s->size_; i++) {
         total += size_t(s->c_str()[i]);
      }
    }

    virtual void done() { }
};

class SumFielder : public Fielder {
public:
    long total;

    SumFielder() { total = 0; }
    ~SumFielder() { }

    void start(size_t r) { }

    void accept(int i) {
      total += i;
    }

    void accept(float i) {
      total += i;
    }

    virtual void done() { }
};

class PrimesFielder : public Fielder {
public:
    long total;

    bool isPrime(int n){
      int m=0;

      for(int i = 2; i <= n/2; ++i) {
        if(n % i == 0) {
              return false;
              break;
        }
      }
      return true;
    }


    PrimesFielder() { total = 0; }
    ~PrimesFielder() { }

    void start(size_t r) { }

    void accept(int i) {
      if(isPrime(i)) {
        total += 1;
      }
    }


    virtual void done() { }
};
