#include <iostream>
#include <fstream>

#include "Token.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "USAGE: ./tokenify <file>" << endl;
        return -1;
    }

    ifstream ifile(argv[1]);

    if (!ifile)
    {
        cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" << endl;
        return -1;
    }


    std::vector<Token> tokens;
    Token tok;

    tok.get(ifile);

    while (tok.type() != EOF_TOK)
    {
        if (tok.type() != ERROR)
        {
            cout << "Resulting token = " << tok << endl;
        }
        else
        {
            cout << "Syntax error detected on line " << tok.lineNumber() << endl;
        }

        tokens.push_back(tok);
        tok.get(ifile);
    }
    char tempfile[] = "tempfile.txt";

    Parser* parser = new Parser(argv[1], tempfile,tokens);
    parser->parse();
    return 0;
}





