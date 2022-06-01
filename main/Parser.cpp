#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "Token.h"
#include "Parser.h"

void ungets(FILE* f, char* s){
	int i;
	for (i = 0; i < (int)strlen(s); i++){
		ungetc(s[strlen(s) - 1 - i], f);
	}
}

void indent(int n, FILE* fout){
	int i;
	for (i = 0; i < n; i++)
		//print out
		fprintf(fout, "\t");
}

int isLineBreaker(char* s){
	return (strcmp(s, ";") == 0);
}

void conditionHeader(FILE* fin, FILE* fout, int cnt){
	char s[100];

	int sc = 0;
	int p = 0;

	while (fscanf(fin, "%s", s) == 1){
		fprintf(fout, "%s ", s);
		if (isLineBreaker(s)){
			sc++;
		}else if (s[0] == '('){
			p++;
		}else if (s[0] == ')'){
			p--;
		}
		if (p == 0 && sc == cnt)
			break;
	}
}

void formatBlock(FILE* fin, FILE* fout, int d, int flg){
	char s[100];
	int nl = 0;

	while (fscanf(fin, "%s", s) == 1){
		std::cout << "called " << s << "\n";
		if (strcmp(s, ";") == 0){
			std::cout << "Found";
			fprintf(fout, "%s\n", s);
			break;
		}

		if (strcmp(s, "end") == 0){
			indent(d - 1, fout);
			fprintf(fout, "%s\n", s);
			break;
		}else if (strcmp(s, "begin") == 0){
			fprintf(fout, "\n");
			indent(d, fout);
			fprintf(fout, "%s\n", s);
			indent(d + 1, fout);
			formatBlock(fin, fout, d + 1, 0);
			nl = 1;
			continue;
		}if (nl){
			indent(d, fout);
			nl = 0;
		}

		fprintf(fout, "%s ", s);

		if (strcmp(s, "for") == 0){
			conditionHeader(fin, fout, 2);
			fprintf(fout, "\n");
			fscanf(fin, "%s", s);
			if (s == "begin"){
				indent(d, fout);
				fprintf(fout, "%s\n", s);
				indent(d + 1, fout);
				formatBlock(fin, fout, d + 1, 0);
			}else if (s == "end"){
				indent(d - 1, fout);
				ungets(fin, s);
				formatBlock(fin, fout, d - 1, 1);
			}
			if (flg)
				break;
			nl = 1;
		}else if (strcmp(s, "while") == 0){
			conditionHeader(fin, fout, 0);
			fprintf(fout, "\n");
			fscanf(fin, "%s", s);
			if (s == "begin"){
				indent(d, fout);
				fprintf(fout, "%s\n", s);
				indent(d + 1, fout);
				formatBlock(fin, fout, d + 1, 0);
			}else if (s == "end"){
				indent(d - 1, fout);
				ungets(fin, s);
				formatBlock(fin, fout, d - 1, 1);
			}
			if (flg)
				break;
			nl = 1;
		}else if (strcmp(s, "if") == 0){
			conditionHeader(fin, fout, 0);
			fprintf(fout, "\n");
			fscanf(fin, "%s", s);
			if (s == "begin"){
				indent(d, fout);
				fprintf(fout, "%s\n", s);
				indent(d + 1, fout);
				formatBlock(fin, fout, d + 1, 0);
			}else if (s == "end"){
				indent(d - 1, fout);
				ungets(fin, s);
				formatBlock(fin, fout, d - 1, 1);
			}

			while (fscanf(fin, "%s", s) == 1){
				if (strcmp(s, "else") == 0){
					indent(d, fout);
					fprintf(fout, "%s ", s);
					fscanf(fin, "%s", s);

					if (strcmp(s, "if") == 0){
						fprintf(fout, "%s ", s);
						conditionHeader(fin, fout, 0);
						fprintf(fout, "\n");
						fscanf(fin, "%s", s);
						if (s == "begin"){
							indent(d, fout);
							fprintf(fout, "%s\n", s);
							indent(d + 1, fout);
							formatBlock(fin, fout, d + 1, 0);
						}else if (s == "end"){
							indent(d - 1, fout);
							ungets(fin, s);
							formatBlock(fin, fout, d - 1, 1);
						}
					}else{
						fprintf(fout, "\n");
						if (s == "begin"){
							indent(d, fout);
							fprintf(fout, "%s\n", s);
							indent(d - 1, fout);
							formatBlock(fin, fout, d -1, 0);
						}else if (s == "end"){
							indent(d - 1, fout);
							ungets(fin, s);
							formatBlock(fin, fout, d - 1, 1);
						}
						nl = 1;
						break;
					}
				}else{
					ungets(fin, s);
					break;
				}
			}

			nl = 1;
			if (flg)
				break;
		}else if (isLineBreaker(s)){
			fprintf(fout, "\n");
			nl = 1;
			if (flg)
				break;
		}
	}
}


//Parser class
Parser::Parser(char* tempfile, char* outputfile, std::vector<Token> t) {
	fin = fopen(tempfile, "r");
	fout = fopen(outputfile, "w");
	tokens = t;
	assert(fin != NULL);
	assert(fout != NULL);
}

//Parser function
void Parser::parse() {
	for (Token i : tokens){
		fprintf(fout, "%s", i.value().c_str());
		switch (i.type()){
		case INTEGER:
			fprintf(fout, " ");
			break;
		case FLOAT:
			fprintf(fout, " ");
			break;
		case VOID:
			fprintf(fout, " ");
			break;
		case COMMA:
			fprintf(fout, " ");
			break;
		case SEMICOLON:
			fprintf(fout, "\n");
			break;
		case BEGIN:
			fprintf(fout, "\t");
			break;
		default:
			break;
		}
	}
	
	fclose(fin);
	fclose(fout);
}
