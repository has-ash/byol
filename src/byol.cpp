#include <cstring>
#include <iostream>

extern "C" {
#include "mpc.h"
#include <editline/readline.h>
}

long eval_op(long x, char* op, long y){
  if(std::strcmp(op, "+") == 0) {return x+y;}
  if(std::strcmp(op, "*") == 0) {return x*y;}
  if(std::strcmp(op, "-") == 0) {return x-y;}
  if(std::strcmp(op, "/") == 0) {return x/y;}
  return 0;
}

long eval(mpc_ast_t *t) {
    // if number then just return number
  if (std::strstr(t->tag, "number")) {
    return std::atoi(t->contents);
  }
  // else return result of eval-ing subtree

  // first child is '('
  char* op = t->children[1]->contents;
  long x = eval(t->children[2]);

  int i = 3;
  while(std::strstr(t->children[i]->tag, "expr")){
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

  mpca_lang(MPCA_LANG_DEFAULT,
            "                                                             \
        number      : /-?[0-9]+/;                                     \
        operator    : '+' | '-' | '*' | '/';                          \
        expr        : <number> | '(' <operator> <expr>+ ')';          \
        lispy       : /^/ <operator> <expr>+ /$/ ;                    \
        ",
            Number, Operator, Expr, Lispy);

  std::cout << "Mlisp Version 0.0.1\n";
  std::cout << "Type 'exit' to exit\n";
  while (true) {
    std::string input = readline("mlisp> ");

    if (input == "exit") {
      break;
    }

    add_history(input.c_str());

    mpc_result_t r;
    if (mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
      long result = eval(static_cast<mpc_ast_t *> (r.output));
      std::cout << result << "\n";
      //mpc_ast_print(static_cast<mpc_ast_t *>(r.output));
      mpc_ast_delete(static_cast<mpc_ast_t *>(r.output));
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
  }

  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  return 0;
}
