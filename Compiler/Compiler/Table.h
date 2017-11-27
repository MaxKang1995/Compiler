#ifndef HEADER_TABLE
#define HEADER_TABLE

#include <string>
#include "Symbol.h"

#define MAX_LEVEL 6

using namespace std;

const enum TokenObject {
    objconstant, objvariable, objprocedure, objfunction
};

const enum TokenType {
    nul, character, integer, arrays
};

const string to2str[] = {
    "constant  ", "variable  ", "procedure ", "function  "
};

const string tt2str[] = {
    "nul       ", "character ", "integer   ", "arrays    "
};

class TokenTableItem {
public:
	string      name;
    TokenObject obj;    // function procedure constant variable
	TokenType   typ;    // int char array
    bool        normal; // false -- var parameter; true -- else
    int         ref;    // refer to other tables(array func proc)
	int         lev;
	int         adr;
    int         link;
};

class ArrayTableItem {
public:
    TokenType type;
    int     length;
};

class BlockTableItem {
public:
    int last  = 0; // index of the last token in tab
    int lpar  = 0; // index of the last par token in tab
    int psize = 0; // parameter size
};

void initTable();
void enter(string, TokenObject, TokenType, int, int = 0, bool = true);
void printtables();

#endif