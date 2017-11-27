#include "Instruction.h"

using namespace std;

extern vector<TokenTableItem> tab;
extern vector<Instruction> instrs;
extern vector<string>        stab;

void printinstructions() {
    ofstream out("result.txt");

    out << "=== Instructions ===" << endl;
    int length = instrs.size();

    for (int i = 0; i < length; i++) {
        Instruction instr = instrs[i];
        string s = instr2str(instr);
        if (instr.op == RET) s += "\n";
        out << s << endl;
    }
    out.close();
}

string instr2str(Instruction instr) {
    string s = op2str[instr.op];
    Operation oper = instr.op;

    if (oper == ADD || oper == SUB || oper == MUL || oper == DIV ||
        oper == BGT || oper == BGE || oper == BEQ || oper == BNE ||
        oper == BLE || oper == BLT || oper == OFF || oper == SEL ||
        oper == PUSH) {
        s += trans(instr.d);
        s += ", " + trans(instr.s1);
        s += ", " + trans(instr.s2);
    }
    else if (oper == NEG || oper == CALF || oper == ASG) {
        s += trans(instr.d);
        s += ", " + trans(instr.s1);
    }
    else if (oper == JMP || oper == CALP || oper == RET || oper == RVAL ||
        oper == READ || oper == WRITE || oper == LABEL) {
        s += trans(instr.d);
    }
    return s;
}

string trans(Operand o) {
    ostringstream ss;
    if (o.type == oconst)
        ss << o.constant;
    else if (o.type == olabel)
        ss << o.label;
    else if (o.type == otemp)
        ss << o.label;
    else if (o.type == oident) {
        string s = tab[o.ident].name;
        int i = s.find_first_of(' ');
        s = s.substr(0, i);
        ss << s;
    }
    else if (o.type == ostring)
        ss << stab[o.str];
    return ss.str();
}