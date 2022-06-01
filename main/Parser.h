#pragma once

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "Token.h"

//Implement functions
void ungets(FILE* f, char* s);
void indent(int n, FILE* fout);
int isLineBreaker(char* s);
void conditionHeader(FILE* fin, FILE* fout, int cnt);
void formatBlock(FILE* fin, FILE* fout, int d, int flg);

//Implement Parser class
class Parser {
private:
	char s[1000];
	FILE* fin, * fout;
	std::vector<Token> tokens;
public:
	Parser(char* tempfile, char* outputfile, std::vector<Token> t);
	void parse();
};
