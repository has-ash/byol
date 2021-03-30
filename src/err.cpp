#include "err.h"

lval lval_num(long x) {
  return lval{lval_type::LVAL_NUM, x, err_type::LERR_NOP};
}

lval lval_err(err_type x) { return lval{lval_type::LVAL_ERR, 0, x}; }

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
