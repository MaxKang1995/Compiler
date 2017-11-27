#ifndef HEADER_LEXER
#define HEADER_LEXER

#define MAX_LENGTH_IDENT  10
#define MAX_LENGTH_NUMBER 10

#include "Symbol.h"
#include <string>

using namespace std;

void getch();
void getsym();

#endif