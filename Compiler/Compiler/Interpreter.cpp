#include "Interpreter.h"

using namespace std;

extern vector<TokenTableItem>  tab;
extern vector<BlockTableItem> btab;
extern vector<Instruction>  instrs;
extern vector<string>         stab;
extern vector<string>         ptab;

ofstream asmfile;

void genAsm(string filename) {
    int addr = 0; // string mem counting
    int locsize = 0;
    stringstream ss;
    asmfile.open(filename);

    // initialize string
    asmfile << "\t.data\n" << endl;
    for (int i = 0; i < stab.size(); i++) {
        ss.str("");
        ss << "s" << i << ":\t.asciiz ";
        addr += stab[i].length() + 1;
        asmfile << ss.str() + stab[i] << endl;
    }
    asmfile << endl;
    addr = addr / UNIT + 1;

    // initialize data stack & global frame
    asmfile << "\t.text" << endl;
    ss.str("");
    ss << addr * UNIT;
    writeAsm("li", "$fp", ss.str());
    writeAsm("li", "$sp", ss.str());
    writeAsm("li", "$t0", ss.str());
    
    ss.str("");

    // alloc memory for local variables
    /*int locvarsize = 0;
    asmfile << "# alloc memory for local variables #" << endl;
    for (int i = btab[0].last; i != btab[tab[0].ref].lpar; i = tab[i].link)
        if (tab[i].obj == objvariable) locvarsize += 4;
    ss.str("");
    ss << locvarsize;
    writeAsm("addi", "$sp", "$sp", ss.str());
    writeAsm("j", "entry");
    */
    int locvarsize = 0;
    int lastvar = btab[0].last;
    for (; lastvar != btab[0].lpar; lastvar = tab[lastvar].link)
        if (tab[lastvar].obj == objvariable) break;
    int fistvar = lastvar;
    for (int i = lastvar; i != btab[0].lpar; i = tab[i].link)
        if (tab[i].obj == objvariable) fistvar = i;
    if (lastvar == btab[0].lpar) // no local variable
        locvarsize = 0;
    else
        locvarsize = tab[lastvar].adr - tab[fistvar].adr + 4;
    ss.str("");
    ss << locvarsize;
    writeAsm("addi", "$sp", "$sp", ss.str());
    writeAsm("j", "entry");


    for (int i = 0; i < instrs.size(); i++) {
        Instruction instr = instrs[i];
        asmfile << "# " << instr2str(instrs[i]) << " #" << endl;

        string temp;

        switch (instr.op) {
        // ADD SUB MUL DIV
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            loadOperand(instr.s1, "$t0", instr.lev);
            loadOperand(instr.s2, "$t1", instr.lev);
            temp = instr.op == ADD ? "add" : instr.op == SUB ? "sub" :
                   instr.op == MUL ? "mul" : "div";
            writeAsm(temp, "$t0", "$t0", "$t1");
            writeAsm("sw", "$t0", loadAddress(instr.d, instr.lev));
            break;

        case NEG:
            loadOperand(instr.s1, "$t0", instr.lev);
            writeAsm("neg", "$t0", "$t0");
            writeAsm("sw", "$t0", loadAddress(instr.d, instr.lev));
            break;

        // BGT BGE BEQ BNE BLE BLT
        case BGT:
        case BGE:
        case BEQ:
        case BNE:
        case BLE:
        case BLT:
            loadOperand(instr.s1, "$t0", instr.lev);
            loadOperand(instr.s2, "$t1", instr.lev);
            temp = instr.op == BGT ? "bgt" : instr.op == BGE ? "bge" :
                instr.op == BEQ ? "beq" : instr.op == BNE ? "bne" :
                instr.op == BLE ? "ble" : "blt";
            writeAsm(temp, "$t0", "$t1", instr.d.label);
            break;

        case JMP:
            writeAsm("j", instr.d.label);
            break;

        case CALP:
        case CALF:
            writeAsm("move", "$t0", "$fp");      // save $fp to $t0
            writeAsm("move", "$fp", "$sp");      // reset $fp
            writeAsm("addi", "$sp", "$sp", "4"); // alloc return value mem
            writeAsm("addi", "$sp", "$sp", "4"); // alloc DL mem
            writeAsm("sw", "$t0", "-4($sp)");    // save last $fp in DL
            dealCall(instr);
            break;

        case RET:
            writeAsm("move", "$sp", "$fp");  // reset $sp
            writeAsm("lw", "$t0", "4($fp)"); // save $fp in DL to $t0
            writeAsm("lw", "$v0", "0($fp)"); // load return value to $v0
            writeAsm("move", "$fp", "$t0");  // reset $fp
            ss.str("");
            // get parameter list size
            ss << btab[tab[instr.d.ident].ref].psize;
            // reset $sp (skip parameters)
            writeAsm("sub", "$sp", "$sp", ss.str());
            // return ra
            writeAsm("jr", "$ra");
            break;

        case SAVE:
            // save current status(registers)
            // only $ra($31), others are useless by now
            writeAsm("addi", "$sp", "$sp", "4");
            writeAsm("sw", "$31", "-4($sp)");
            break;

        case PUSH:
            // value parameter
            if (tab[instr.d.ident + instr.s1.constant].normal)
                loadOperand(instr.s2, "$t0", instr.lev);
            // address parameter
            else if (i > 0 && instrs[i - 1].op == OFF) // already got offset [OFF]
                writeAsm("lw", "$t0", loadAddress(instr.s2, instr.lev));
            else
                writeAsm("la", "$t0", loadAddress(instr.s2, instr.lev));
            writeAsm("sw", "$t0", "0($sp)");
            writeAsm("addi", "$sp", "$sp", "4");
            break;

        case RVAL:
            // load return value to $t0
            loadOperand(instr.d, "$t0", instr.lev);
            // rval adr = 0 + $fp
            writeAsm("sw", "$t0", "0($fp)");
            break;

        case READ:
            // read integer : 5, $v0 contains integer read
            // read character : 12, $v0 contains character read
            if (tab[instr.d.ident].typ == integer)
                writeAsm("li", "$v0", "5");
            else writeAsm("li", "$v0", "12");
            asmfile << "syscall" << endl;
            // save what was read
            writeAsm("sw", "$v0", loadAddress(instr.d, instr.lev));
            break;

        case WRITE:
            // print string : 4, $v0 contains string print
            // $a0 : address of null-terminated string to print
            if (instr.d.type == ostring) {
                writeAsm("li", "$v0", "4");
                ss.str("");
                ss << "s" << instr.d.str;
                writeAsm("la", "$a0", ss.str());
            }
            else if (instr.d.type == oconst) {
                // print const integer
                writeAsm("li", "$v0", "1");
                ss.str("");
                ss << instr.d.constant;
                writeAsm("li", "$a0", ss.str());
            }
            else {
                // print integer : 1, $v0 contains integer print
                // print character : 11, $v0 contains character print
                if (tab[instr.d.ident].typ == integer)
                    writeAsm("li", "$v0", "1");
                else writeAsm("li", "$v0", "11");
                // constant or variable
                if (tab[instr.d.ident].obj == objconstant) {
                    ss.str("");
                    ss << tab[instr.d.ident].adr;
                    writeAsm("li", "$a0", ss.str());
                }
                else loadOperand(instr.d, "$a0", instr.lev);
            }
            asmfile << "syscall" << endl;
            break;

        case LABEL:
            writeAsm(instr.d.label);
            break;

        case OFF:
        case SEL:
            // load s1 addr to $t0
            writeAsm("la", "$t0", loadAddress(instr.s1, instr.lev));
            // load s2 value to $t1 (offset)
            loadOperand(instr.s2, "$t1", instr.lev);
            // calculate addr = s1.adr + offset * 4
            writeAsm("mul", "$t1", "$t1", "4");
            writeAsm("add", "$t0", "$t0", "$t1");
            // save addr
            if (instr.op == OFF)
                writeAsm("sw", "$t0", loadAddress(instr.d, instr.lev, false));
            // save value
            else {
                writeAsm("lw", "$t0", "0($t0)");
                writeAsm("sw", "$t0", loadAddress(instr.d, instr.lev));
            }
            break;

        case ASG:
            // set value to $t0 and save it to given addr
            loadOperand(instr.s1, "$t0", instr.lev);
            writeAsm("sw", "$t0", loadAddress(instr.d, instr.lev));
            break;

        }
    }
}

string loadAddress(Operand &o, int l, bool val) {
    stringstream ss;

    // at current level, use current $fp
    if (tab[o.ident].lev == l) {
        ss.str("");
        ss << tab[o.ident].adr << "($fp)";
        // var parameter
        if (!tab[o.ident].normal && val) {
            writeAsm("lw", "$t2", ss.str());
            ss.str("");
            ss << "0($t2)";
        }
    }
    // else use SL to fetch the address
    else {
        ss.str("");
        ss << (tab[o.ident].lev + 1) * UNIT << "($fp)";
        writeAsm("lw", "$t2", ss.str());
        if (!tab[o.ident].normal && val) {
            ss.str(""); ss << tab[o.ident].adr << "($t2)";
            writeAsm("lw", "$t2", ss.str());
            ss.str(""); ss << "0($t2)";
        }
        else {
            ss.str(""); ss << tab[o.ident].adr << "($t2)";
        }
    }
    return ss.str();
}

void loadOperand(Operand &o, string r, int l) {
    stringstream ss;

    // constant : 1, 2, 3...
    if (o.type == oconst) {
        ss << o.constant;
        writeAsm("li", r, ss.str());
    }
    // const ident
    else if (tab[o.ident].obj == objconstant) {
        ss << tab[o.ident].adr;
        writeAsm("li", r, ss.str());
    }
    // variable
    else writeAsm("lw", r, loadAddress(o, l));
}

void dealCall(Instruction instr) {
    stringstream ss;
    int lowlevel = instr.lev - 1 < tab[instr.d.ident].lev ? 
        instr.lev - 1 : tab[instr.d.ident].lev;

    // copy SL from previous frame
    asmfile << "# copy SLs from previous frame #" << endl;
    for (int i = 0; i < lowlevel; i++) {
        ss.str("");
        ss << (i + 2) * UNIT;
        writeAsm("lw", "$t1", ss.str() + "($t0)");
        writeAsm("sw", "$t1", "0($sp)");
        writeAsm("addi", "$sp", "$sp", "4");
    }

    // if call inner block, append new static link
    asmfile << "# append new SL #" << endl;
    if (instr.lev - 1 <= tab[instr.d.ident].lev) {
        writeAsm("sw", "$t0", "0($sp)");
        writeAsm("addi", "$sp", "$sp", "4");
    }

    // alloc memory for local variables
    // pay attention to array size
    /*int locvarsize = 0;
    asmfile << "# alloc memory for local variables #" << endl;
    for (int i = btab[tab[instr.d.ident].ref].last;
        i != btab[tab[instr.d.ident].ref].lpar; i = tab[i].link)
        if (tab[i].obj == objvariable) locvarsize += 4;
    ss.str("");
    ss << locvarsize;
    */
    int locvarsize = 0;
    int lastvar = btab[tab[instr.d.ident].ref].last;
    for (; lastvar != btab[tab[instr.d.ident].ref].lpar; lastvar = tab[lastvar].link)
        if (tab[lastvar].obj == objvariable) break;
    int fistvar = lastvar;
    for (int i = lastvar; i != btab[tab[instr.d.ident].ref].lpar; i = tab[i].link)
        if (tab[i].obj == objvariable) fistvar = i;
    if (lastvar == btab[tab[instr.d.ident].ref].lpar) // no local variable
        locvarsize = 0;
    else
        locvarsize = tab[lastvar].adr - tab[fistvar].adr + 4;
    ss.str("");
    ss << locvarsize;

    writeAsm("addi", "$sp", "$sp", ss.str());
    // jump and link to procedure/function
    writeAsm("jal", ptab[tab[instr.d.ident].adr]);
    // after returning, restore previous status of registers
    writeAsm("lw", "$31", "-4($sp)");
    writeAsm("subi", "$sp", "$sp", "4");
    // if calling function, save return value
    if (instr.op == CALF)
        writeAsm("sw", "$v0", loadAddress(instr.s1, instr.lev));
}
void writeAsm(string op, string o1, string o2, string o3) {
    asmfile << op;
    if (o1 == "") { // label
        assert(o2 == "" && o3 == "");
        asmfile << ':' << endl;
    }
    else {
        asmfile << '\t' << o1;
        if (o2 != "") asmfile << ", " << o2;
        if (o3 != "") asmfile << ", " << o3;
        asmfile << endl;
    }
}