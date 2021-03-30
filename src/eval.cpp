#include "eval.h"
#include <cstring>

lval eval_op(lval x, char *op, lval y) {
  if (x.type == lval_type::LVAL_ERR) {
    return x;
  } else if (y.type == lval_type::LVAL_ERR) {
    return y;
  } else {

    if (std::strcmp(op, "+") == 0) {
      return lval_num(x.num + y.num);

    } else if (std::strcmp(op, "*") == 0) {
      return lval_num(x.num * y.num);

    } else if (std::strcmp(op, "-") == 0) {
      return lval_num(x.num - y.num);

    } else if (std::strcmp(op, "/") == 0) {
      return y.num == 0 ? lval_err(err_type::LERR_DIV_ZERO)
                        : lval_num(x.num / y.num);

    } else if (std::strcmp(op, "%") == 0) {
      return lval_num(x.num % y.num);

    } else if (std::strcmp(op, "^") == 0) {
      return lval_num(std::pow(x.num, y.num));

    } else if (std::strcmp(op, "min") == 0) {
      return x.num < y.num ? lval_num(x.num) : lval_num(y.num);

    } else if (std::strcmp(op, "max") == 0) {
      return x.num > y.num ? lval_num(x.num) : lval_num(y.num);
    }
  }
  return lval_err(err_type::LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
  // if number then just return number
  if (std::strstr(t->tag, "number")) {
    // errno is a macro that can be set by standard library functions
    // to the error number macros defined in cerrno
    errno = 0;
    long x = std::strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(err_type::LERR_BAD_NUM);
  }
  // else return result of eval-ing subtree

  // first child is '('
  char *op = t->children[1]->contents;
  lval x = eval(t->children[2]);

  int i = 3;
  while (std::strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  return x;
}
