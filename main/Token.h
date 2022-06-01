#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <iostream>
#include <string>
#include <vector>

enum TokenType { ERROR, EOF_TOK, NUM_INT, NUM_REAL, ADDOP, MULOP, ID, RELOP, ASSIGNOP, LPAREN, RPAREN, SEMICOLON, LBRACK, RBRACK, COMMA, AND, OR, INTEGER, FLOAT, WHILE, IF, THEN, ELSE, VOID, BEGIN, END };

extern std::string TokStr[];

class Token{
private:
	std::string _value;      // actual lexeme value for the Token
	TokenType _type;         // token type, from the above enumeration
	unsigned int _line_num;  // line number token is found on
public:
	Token() :_value(), _type(ERROR), _line_num(0) {}

	TokenType type() const { return _type; }
	TokenType& type() { return _type; }
	const std::string& value() const { return _value; }
	std::string& value() { return _value; }

	unsigned int lineNumber() const { return _line_num; }
	unsigned int& lineNumber() { return _line_num; }


	void get(std::istream& is);

	std::ostream& print(std::ostream& os) const;

	short int operator==(const TokenType tt) { return _type == tt; }
	short int operator!=(const TokenType tt) { return !(*this == tt); }

	short int operator==(const std::string& s) { return _value == s; }
	short int operator!=(const std::string& s) { return !(*this == s); }
};

inline std::ostream& operator<<(std::ostream& os, const Token& t)
{
	return t.print(os);
}

#endif
