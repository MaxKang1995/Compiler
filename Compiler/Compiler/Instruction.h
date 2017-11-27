#ifndef HEADER_INSTRUCTION
#define HEADER_INSTRUCTION

#include "Table.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

extern int level;

const enum Operation {
    ADD, SUB, MUL, DIV, NEG,
    JMP, BGT, BGE, BEQ, BNE, BLE, BLT,
    CALP, CALF, RET, SAVE, PUSH, RVAL,
    READ, WRITE, LABEL, OFF, SEL, ASG 
};

const string op2str[] = {
    "ADD   ", "SUB   ", "MUL   ", "DIV   ", "NEG   ",
    "JMP   ", "BGT   ", "BGE   ", "BEQ   ", "BNE   ", "BLE   ", "BLT   ",
    "CALP  ", "CALF  ", "RET   ", "SAVE  ", "PUSH  ", "RVAL  ",
    "READ  ", "WRITE ", "LABEL ", "OFF   ", "SEL   ", "ASG   "
};

const enum OperandType {
    oconst, otemp, oident, olabel, ostring
};

class Operand {
public:
    int    constant;
    int    ident;
    string label;
    int    str;
    OperandType type;
};

class Instruction {
public:
    Operation op;
    Operand   d;
    Operand   s1;
    Operand   s2;
    int       lev;
    Instruction(Operation op, Operand d = Operand(), Operand s1 = Operand(),
        Operand s2 = Operand()) {
        this->lev = level;
        this->op  = op;
        this->d   = d;
        this->s1  = s1;
        this->s2  = s2;
    };
};

void printinstructions();
string instr2str(Instruction);
string trans(Operand);

#endif