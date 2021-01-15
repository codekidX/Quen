#include <iostream>
using std::cout;
using std::endl;

#include "lexer.h"
// #include "parser.h"
#include "parser.h"

int main()
{
    Lexer lexer("main.qn");
    lexer.read_from_file();
    lexer.analyze();

    // TODO: create a basic logger for the whole project and run logs
    // on debug mode
    // for (auto def : lexer.get_tokens())
    // {
    //     cout << " - " << def.token_str << " | " << def.value << endl;
    // }

    Parser parser(lexer.get_tokens());
    parser.parse();

    return 0;
}