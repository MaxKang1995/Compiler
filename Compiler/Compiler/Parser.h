#ifndef HEADER_PARSER
#define HEADER_PARSER

#include "Symbol.h"
#include "Instruction.h"
#include "Table.h"
#include "Lexer.h"
#include "Error.h"
#include "Interpreter.h"
#include <set>

using namespace std;

class ConstRec {
public:
    TokenType type;
    int      value;
};

void parser(string);
void block(set<Symbol>, bool, Operand);
void test(set<Symbol>, set<Symbol>, int);
void testsemicolon(set<Symbol>, set<Symbol>);
void parameterlist(set<Symbol>);
void constdeclaration(set<Symbol>);
void vardeclaration(set<Symbol>);
void procdeclaration(set<Symbol>);
void statement(set<Symbol>);
void callstatement(set<Symbol>, int, Operand &);
void assignment(set<Symbol>);
void compoundstatement(set<Symbol>);
void ifstatement(set<Symbol>);
void casestatement(set<Symbol>);
void caselabel(set<Symbol>, TokenType, Operand &);
void onecase(set<Symbol>, TokenType, Operand, Operand);
void forstatement(set<Symbol>);
void readstatement(set<Symbol>);
void writestatement(set<Symbol>);
void condition(set<Symbol>, Operand &);
TokenType expression(set<Symbol>, Operand &);
TokenType selector(set<Symbol>, Operand &);
TokenType term(set<Symbol>, Operand &);
TokenType factor(set<Symbol>, Operand &);
TokenType resulttype(TokenType, TokenType);
int loc(string, bool = true);
void info(string);
ConstRec constant(set<Symbol>);
bool isStrLegal(string);

#endif