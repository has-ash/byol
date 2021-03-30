#include <cmath>
#include <cstring>
#include <iostream>

using std::cout;

extern "C" {
#include "mpc.h"
#include <editline/readline.h>
}

// TODO: consider using std::variant instead of lval struct.
// Seems to make a decent amount of sense to use that instead.

// for the lval.type field
enum class lval_type { LVAL_NUM, LVAL_ERR };
// for the lval.err field; LERR_NOP is a dummy value for creating
// structs that have type==lval_type::LVAL_NUM i.e. it indicates
// that the struct does not repsent an error.
enum class err_type { LERR_NOP, LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

// lval: lisp value (either valid value or error)
// what is the difference between type and err?
// type==0: valid number, type==1: error
struct lval {
  lval_type type;
  long num;
  err_type err;

  friend std::ostream &operator<<(std::ostream &os, lval to_print) {
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
};

// Perhaps refactor: would a constructor be a better idea?
// although two constructors overloaded on long/int do not seem like a great
// idea
lval lval_num(long x) {
  return lval{lval_type::LVAL_NUM, x, err_type::LERR_NOP};
}

lval lval_err(err_type x) { return lval{lval_type::LVAL_ERR, 0, x}; }

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

int main() {

  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  mpca_lang(
      MPCA_LANG_DEFAULT,
      "                                                                            \
        number      : /-?[0-9]+/;                                                  \
        operator    : '+' | '-' | '*' | '/' | '%' | '^' ;                          \
        expr        : <number> | '(' <operator> <expr>+ ')';                       \
        lispy       : /^/ <operator> <expr>+ /$/ ;                                 \
        ",
      Number, Operator, Expr, Lispy);

  cout << "Mlisp Version 0.0.1\n";
  cout << "Type 'exit' to exit\n";
  while (true) {
    std::string input = readline("mlisp> ");

    if (input == "exit") {
      break;
    }

    add_history(input.c_str());

    mpc_result_t r;
    if (mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
      lval result = eval(static_cast<mpc_ast_t *>(r.output));
      cout << result << "\n";
      // mpc_ast_print(static_cast<mpc_ast_t *>(r.output));
      mpc_ast_delete(static_cast<mpc_ast_t *>(r.output));
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
  }

  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  return 0;
}
