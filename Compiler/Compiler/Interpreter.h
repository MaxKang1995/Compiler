#ifndef HEADER_INTERPRETER
#define HEADER_INTERPRETER

#define UNIT 4

#include "Instruction.h"
#include "Table.h"
#include <vector>
#include <string>
#include <assert.h>

string loadAddress(Operand &, int, bool = true);
void loadOperand(Operand &, string, int);
void dealCall(Instruction);
void genAsm(string path);
void writeAsm(string, string = "", string = "", string = "");

#endif