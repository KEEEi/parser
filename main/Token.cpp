#include "Token.h"

#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
using namespace std;

//declare automata function
static int automata(int state, int input);

//declare final_state_to_token_type function
static enum TokenType final_state_to_token_type(int final_state, const string& token_value);

// the promised global for string equivalents of TokenType enumeration
string TokStr[] =
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",
   "SEMICOLON", "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE",
   "VOID", "BEGIN", "END" };


static string reserved[] = { "int", "float", "while", "if", "then", "else", "void", "begin", "end" };

static bool is_final_state[] = { false, true, true, false, true, true, true, true, true, true, true, true, false, true,
                                false, true, true, true, true, true };

ostream&
Token::print(ostream& os) const {
    os
        << "{ Type:" << left << setw(10) << TokStr[_type]
        << " Value:" << left << setw(10) << _value
        << " Line Number:" << _line_num
        << " }";
    return os;
}

void Token::get(istream& is) {
    int state = 0;
    string token_value;
    int c;
    while ((c = is.get()) != ifstream::traits_type::eof()) {
        int new_state = automata(state, c);
        if (new_state == -1) {
            if (state == 20 || state == 21 || state == 22) {
                is.unget();
                state = 0;
                token_value.clear();
            }
            else if (is_final_state[state]) {
                is.unget();
                _value = token_value;
                _type = final_state_to_token_type(state, token_value);
                return;
            }
            else {
                if (c == '\n') {
                    _line_num++;
                }
                _type = ERROR;
                return;
            }
        }
        else {
            token_value.push_back(c);
            if (c == '\n') {
                _line_num++;
            }
            state = new_state;
        }
    }
    if (state == 20 || state == 21 || state == 22) {
        _type = EOF_TOK;
    }
    else if (token_value.empty()) {
        _type = EOF_TOK;
    }
    else if (is_final_state[state]) {
        _value = token_value;
        _type = final_state_to_token_type(state, token_value);
    }
    else {
        _type = ERROR;
    }
}

//Function which returns token string from final_state
static enum TokenType final_state_to_token_type(int final_state, const string& token_value) {
    switch (final_state) {
    case 1:
        if (token_value == "void") {
            return VOID;
        }
        else if (token_value == "int") {
            return INTEGER;
        }
        else if (token_value == "float") {
            return FLOAT;
        }
        else if (token_value == "begin") {
            return BEGIN;
        }
        else if (token_value == "end") {
            return END;
        }
        else if (token_value == "if") {
            return IF;
        }
        else if (token_value == "then") {
            return THEN;
        }
        else if (token_value == "else") {
            return ELSE;
        }
        else if (token_value == "while") {
            return WHILE;
        }
        else {
            return ID;
        }
    case 2:
        return NUM_INT;
    case 4:
        return NUM_REAL;
    case 5:
        return ADDOP;
    case 6:
        return MULOP;
    case 7:
        return RELOP;
    case 8:
        return RELOP;
    case 9:
        return ASSIGNOP;
    case 10:
        return LPAREN;
    case 11:
        return RPAREN;
    case 13:
        return AND;
    case 15:
        return OR;
    case 16:
        return SEMICOLON;
    case 17:
        return LBRACK;
    case 18:
        return RBRACK;
    case 19:
        return COMMA;
    }
    return ERROR;
}

//function which checks input types and returns each numbers from state and input
static int automata(int state, int input) {
    if (state == 0) {
        if (isalpha(input)) {
            return 1;
        }
        if (isdigit(input)) {
            return 2;
        }
        if (input == '+' || input == '-') {
            return 5;
        }
        if (input == '*' || input == '/') {
            return 6;
        }
        if (input == '<' || input == '>') {
            return 7;
        }
        if (input == '=') {
            return 9;
        }
        if (input == '(') {
            return 10;
        }
        if (input == ')') {
            return 11;
        }
        if (input == '&') {
            return 12;
        }
        if (input == '|') {
            return 14;
        }
        if (input == ';') {
            return 16;
        }
        if (input == '[') {
            return 17;
        }
        if (input == ']') {
            return 18;
        }
        if (input == ',') {
            return 19;
        }
        if (input == '#') {
            return 20;
        }
        if (isspace(input)) {
            return 21;
        }
    }
    else if (state == 1) {
        if (isalnum(input)) {
            return 1;
        }
    }
    else if (state == 2) {
        if (isdigit(input)) {
            return 2;
        }
        if (input == '.') {
            return 3;
        }
    }
    else if (state == 3) {
        if (isdigit(input)) {
            return 4;
        }
    }
    else if (state == 4) {
        if (isdigit(input)) {
            return 4;
        }
    }
    else if (state == 7) {
        if (input == '=') {
            return 8;
        }
    }
    else if (state == 9) {
        if (input == '=') {
            return 8;
        }
    }
    else if (state == 12) {
        if (input == '&') {
            return 13;
        }
    }
    else if (state == 14) {
        if (input == '|') {
            return 15;
        }
    }
    else if (state == 20) {
        if (input == '\n') {
            return 22;
        }
        else {
            return 20;
        }
    }
    else if (state == 21) {
        if (isspace((input))) {
            return 21;
        }
    }
    return -1;
}

