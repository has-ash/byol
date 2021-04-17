#include "err.h"

#include <utility>

lval *lval_num(long x) {
  lval *v = new lval;
  v->type = lval_type::LVAL_NUM;
  v->num = x;
  return v;
}

lval *lval_err(std::string err_msg) {
  lval *v = new lval;
  v->type = lval_type::LVAL_ERR;
  v->err = std::move(err_msg);
  return v;
}

lval *lval_sym(std::string symbol) {
  lval *v = new lval;
  v->type = lval_type::LVAL_SYM;
  v->sym = std::move(symbol);
  return v;
}

lval *lval_sexpr(void) {
  lval *v = new lval;
  v->type = lval_type::LVAL_SEXPR;
  v->count = 0;
  return v;
}

void lval_del(lval *v) {
  if (v->type == lval_type::LVAL_SEXPR) {
    for (int i = 0; i < v->count; i++) {
      lval_del(v->cell[i]);
    }
    delete v->cell;
  }

  // delete memory allocated for v itself
  // TODO: I really should use RAII for this
  // or maybe even a better design
  delete v;
}

std::ostream &operator<<(std::ostream &os, lval to_print) {
  switch (to_print.type) {
  case lval_type::LVAL_NUM:
    os << to_print.num;
    break;

  case lval_type::LVAL_ERR:
    if (to_print.err == err_type::LERR_DIV_ZERO) {
      os << "Error: Division by zero!";
    } else if (to_print.err == err_type::LERR_BAD_OP) {
      os << "Error: Invalid Operator";
    } else if (to_print.err == err_type::LERR_BAD_NUM) {
      os << "Error: Invalid Number!";
    }
    break;
  }
  os << "\n";
  return os;
}
