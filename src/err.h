#ifndef __ERR_H_
#define __ERR_H_

#include <iostream>
#include <string>

// TODO: consider using std::variant instead of lval struct.
// Seems to make a decent amount of sense to use that instead.

// for the lval.type field
enum class lval_type { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR };
// for the lval.err field; LERR_NOP is a dummy value for creating
// structs that have type==lval_type::LVAL_NUM i.e. it indicates
// that the struct does not repsent an error.
enum class err_type { LERR_NOP, LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

struct lval {
  lval_type type;
  long num;

  // error and symbol as strings
  std::string err;
  std::string sym;

  // due to sexpr, maintain ptr to list of lvals
  // TODO: May need to refactor this later once I have
  // the complete picture
  int count;
  lval **cell;

  friend std::ostream &operator<<(std::ostream &os, lval to_print);
};

lval *lval_num(long x);
lval *lval_err(err_type x);

#endif // __ERR_H_
