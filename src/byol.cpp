#include <iostream>

extern "C"{
#include<editline/readline.h>
#include<mpc.h>
}

int main() 
{

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                             \
        number      : /-?[0-9]+/;                                     \
        operator    : '+' | '-' | '*' | '/';                          \
        expr        : <number> | '(' <operator> <expr>+ ')';          \
        lispy       : /^/ <operator> <expr>+ /$/ ;                    \
        ",
        Number, Operator, Expr, Lispy);

    std::cout << "Mlisp Version 0.0.1\n" ;
    std::cout << "Type 'exit' to exit\n" ;
    while(true)
    {
        std::string input = readline("mlisp> ");

        if(input == "exit")
        {
            break;
        }

        add_history( input.c_str() );

        mpc_result_t r;
        if(mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
            mpc_ast_print(static_cast<mpc_ast_t*>(r.output));
            mpc_ast_delete(static_cast<mpc_ast_t*>(r.output));
        } else{
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

    }

    mpc_cleanup(4, Number, Operator, Expr, Lispy);
    return 0;
}
