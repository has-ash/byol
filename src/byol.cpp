#include "err.h"
#include "eval.h"
#include <cmath>
#include <iostream>

extern "C" {
#include "mpc.h"
#include <editline/readline.h>
}

using std::cout;

// TODO: Fetch "mpc" using cmake
// TODO: Tests
// TODO: Github CI

int main() {

  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  mpca_lang(
      MPCA_LANG_DEFAULT,
      "                                                                            \
        number      : /-?[0-9]+/;                                                  \
        symbol      : '+' | '-' | '*' | '/' | '%' | '^' ;                          \
		sexpr       : '(' <expr>* ')'; 												\
        expr        : <number> | <symbol> |  <sexpr>;                       		\
        lispy       : /^/ <expr>* /$/ ;                                 			\
        ",
      Number, Symbol, Sexpr, Expr, Lispy);

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

  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);
  return 0;
}
