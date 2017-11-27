#ifndef HEADER_SYMBOL
#define HEADER_SYMBOL

#include <map>
#include <set>

using namespace std;

const enum Symbol {
    NUL, IDENT, NUMBER, CONSTCHAR, CONSTSTR, ARRAY, CHAR, INTEGER, CONSTSYM, VARSYM, PROCSYM, FUNSYM, FORSYM, DOWNTOSYM,
    TOSYM, IFSYM, THENSYM, ELSESYM, CASESYM, OFSYM, DOSYM, BEGINSYM, ENDSYM, READSYM, WRITESYM,
	PLUS, MINUS, TIMES, SLASH, LPAREN, RPAREN, SINGQUO, DOUBQUO, LSS, LEQ, GTR, GEQ, EQL, NEQ, COMMA, SEMICOLON, 
    COLON, PERIOD, LBRACKET, RBRACKET, BECOMES,
};

const string sym2str[] = {
    "NUL       ", "IDENT     ", "NUMBER    ", "CONSTCHAR ", "CONSTSTR  ", "ARRAY     ", "CHAR      ", "INTEGER   ",
    "CONSTSYM  ", "VARSYM    ", "PROCSYM   ", "FUNSYM    ", "FORSYM    ", "DOWNTOSYM ", "TOSYM     ", "IFSYM     ",
    "THENSYM   ", "ELSESYM   ", "CASESYM   ", "OFSYM     ", "DOSYM     ", "BEGINSYM  ", "ENDSYM    ", "READSYM   ",
    "WRITESYM  ", "PLUS      ", "MINUS     ", "TIMES     ", "SLASH     ", "LPAREN    ", "RPAREN    ", "SINGQUO   ",
    "DOUBQUO   ", "LSS       ", "LEQ       ", "GTR       ", "GEQ       ", "EQL       ", "NEQ       ", "COMMA     ",
    "SEMICOLON ", "COLON     ", "PERIOD    ", "LBRACKET  ", "RBRACKET  ", "BECOMES   ", 
};

const map<string, Symbol> str2sym = {
	map<string, Symbol>::value_type("const     ", CONSTSYM),
	map<string, Symbol>::value_type("var       ", VARSYM),
	map<string, Symbol>::value_type("procedure ", PROCSYM),
	map<string, Symbol>::value_type("function  ", FUNSYM),
	map<string, Symbol>::value_type("for       ", FORSYM),
	map<string, Symbol>::value_type("downto    ", DOWNTOSYM),
	map<string, Symbol>::value_type("to        ", TOSYM),
	map<string, Symbol>::value_type("if        ", IFSYM),
	map<string, Symbol>::value_type("then      ", THENSYM),
	map<string, Symbol>::value_type("else      ", ELSESYM),
	map<string, Symbol>::value_type("case      ", CASESYM),
    map<string, Symbol>::value_type("of        ", OFSYM),
    map<string, Symbol>::value_type("do        ", DOSYM),
	map<string, Symbol>::value_type("begin     ", BEGINSYM),
	map<string, Symbol>::value_type("end       ", ENDSYM),
	map<string, Symbol>::value_type("read      ", READSYM),
	map<string, Symbol>::value_type("write     ", WRITESYM),
	map<string, Symbol>::value_type("integer   ", INTEGER),
	map<string, Symbol>::value_type("char      ", CHAR),
	map<string, Symbol>::value_type("array     ", ARRAY)
};

const map<char, Symbol> char2sym = {
	map<char, Symbol>::value_type('+', PLUS),
	map<char, Symbol>::value_type('-', MINUS),
	map<char, Symbol>::value_type('*', TIMES),
	map<char, Symbol>::value_type('/', SLASH),
	map<char, Symbol>::value_type('(', LPAREN),
	map<char, Symbol>::value_type(')', RPAREN),
	map<char, Symbol>::value_type('<', LSS),
	map<char, Symbol>::value_type('>', GTR),
	map<char, Symbol>::value_type('=', EQL),
	map<char, Symbol>::value_type(',', COMMA),
	map<char, Symbol>::value_type(';', SEMICOLON),
	map<char, Symbol>::value_type(':', COLON),
	map<char, Symbol>::value_type('.', PERIOD),
	map<char, Symbol>::value_type('[', LBRACKET),
	map<char, Symbol>::value_type(']', RBRACKET)
};

#endif
