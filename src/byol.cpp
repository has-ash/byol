#include <iostream>

extern "C"{
#include<editline/readline.h>
#include<mpc.h>
}

int main() 
{
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

        std::cout << "mlisp> you typed: " + input << '\n';
    }

    return 0;
}
