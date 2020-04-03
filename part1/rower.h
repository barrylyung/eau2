#include "row.h"

#pragma once

/*******************************************************************************
 *  Rower::
 *  An interface for iterating through each row of a data frame. The intent
 *  is that this class should subclassed and the accept() method be given
 *  a meaningful implementation. Rowers can be cloned for parallel execution.
 */
class Rower : public Object {
 public:
  virtual ~Rower() {}
  /** This method is called once per row. The row object is on loan and
      should not be retained as it is likely going to be reused in the next
      call. The return value is used in filters to indicate that a row
      should be kept. */
  virtual bool accept(Row& r) { return true; }

  /** Once traversal of the data frame is complete the rowers that were
      split off will be joined.  There will be one join per split. The
      original object will be the last to be called join on. The join method
      is reponsible for cleaning up memory. */
  void join_delete(Rower* other) {
      join_delete_(other);
  }

  virtual void join_delete_(Rower* other) { }
};

/**
 * Rower subclass that prints a row in sor format
 * authors: hughes.ni@husky.neu.edu, schoenberger.a@husky.neu.edu
 */
class PrintRower : public Rower {
 public:
  PrintFielder* f;  // PrintFielder used to print each row

  PrintRower() { f = new PrintFielder(); }

  ~PrintRower() { delete f; }

  bool accept(Row& r) {
    r.visit(0, *f);
    return true;
  }

  Object* clone() {
    return new PrintRower();
  }

  void join_delete_(Rower* other) {
    if (!other) {
        exit(1);
    }
    PrintRower* r = dynamic_cast<PrintRower*>(other);
    if (!r) {
        exit(1);
    }
    delete r;
  }

};

class SumRower: public Rower {
public:
    SumFielder* f;
    long total;

    SumRower() {
        total = 0;
        f = new SumFielder();
    }

    SumRower(long t) {
        total = t;
        f = new SumFielder();
    }

    ~SumRower() { delete f; }

    bool accept(Row& r) {
        r.visit(0, *f);
        total = f->total;
        return true;
    }

    void join_delete_(Rower* other) {
        if (!other) {
            exit(1);
        }

        SumRower* r = dynamic_cast<SumRower*>(other);
        if (!r) {
            exit(1);
        }

        total += r->total;
        delete r;
    }

    Object* clone() {
        return new SumRower(total);
    }
};

class ValOfStrings: public Rower {
public:
    ValStringsFielder* f;
    long total;

    ValOfStrings() {
        total = 0;
        f = new ValStringsFielder();
    }

    ValOfStrings(long t) {
        total = t;
        f = new ValStringsFielder();
    }

    ~ValOfStrings() { delete f; }

    bool accept(Row& r) {
        r.visit(0, *f);
        total = f->total;
        return true;
    }

    void join_delete_(Rower* other) {
        if (!other) {
            exit(1);
        }

        ValOfStrings* r = dynamic_cast<ValOfStrings*>(other);
        if (!r) {
            exit(1);
        }

        total += r->total;
        delete r;
    }

    Object* clone() {
        return new ValOfStrings(total);
    }
};

class CountPrimes: public Rower {
public:
    PrimesFielder* f;
    long total;

    CountPrimes() {
        total = 0;
        f = new PrimesFielder();
    }

    CountPrimes(long t) {
        total = t;
        f = new PrimesFielder();
    }

    ~CountPrimes() { delete f; }

    bool accept(Row& r) {
        r.visit(0, *f);
        total = f->total;
        return true;
    }

    void join_delete_(Rower* other) {
        if (!other) {
            exit(1);
        }

        CountPrimes* r = dynamic_cast<CountPrimes*>(other);
        if (!r) {
            exit(1);
        }

        total += r->total;
        delete r;
    }

    Object* clone() {
        return new CountPrimes(total);
    }
};
