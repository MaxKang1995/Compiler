#include "Parser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

extern string word;
extern string id;
extern Symbol sym;
extern int number;
extern int errnum;
extern int display[];
extern vector<TokenTableItem> tab;
extern vector<ArrayTableItem> atab;
extern vector<BlockTableItem> btab;
extern vector<string>         stab;
extern vector<string>         ptab;

const set<Symbol> blockbegsys = { CONSTSYM, VARSYM, PROCSYM, FUNSYM, BEGINSYM };
const set<Symbol> statebegsys = { BEGINSYM, IFSYM, CASESYM, READSYM, WRITESYM, FORSYM };
const set<Symbol> constbegsys = { PLUS, MINUS, CONSTCHAR, NUMBER };
const set<Symbol> facbegset = { IDENT, NUMBER, LPAREN };

int mempoint[MAX_LEVEL + 1];  // alloc mem point

vector<Instruction> instrs;
int tempi = 0;
int label = 0;

ifstream src;
int level = 0;

void parser(string filename) {
    src.open(filename);
    if (!src) fatal(1); // fatal finding src file

    getch();
    getsym();
    initTable();
    block(blockbegsys, false, Operand());
    if (sym == PERIOD) info("\n=== COMPILE FINISHED ===");
    else error(38);

    src.close();
    // printtables();
    // printinstructions();
    if (errnum != 0) fatal(2);

    genAsm("Target_code.asm");
}

void block(set<Symbol> fsys, bool isFunc, Operand lab) {
    info("enter block");

    set<Symbol> temp;
    level++;

    // mem pointer reset
    // give ret value/DL each 1 unit
    // give SL (level - 1) * UNIT units
    // lev1 : 0, lev2 : 1, lev3 : 2
    // mempoint[level] = (level + 1) * UNIT;// +2 * UNIT;
    if (level == 1) mempoint[level] = 0;
    else mempoint[level] = (level + 1) * UNIT;// +2 * UNIT;
    if (level > MAX_LEVEL) fatal(0);

    btab.push_back(BlockTableItem());
    display[level] = btab.size() - 1;

    if (level != 1) {
        int index = tab.size() - 1;
        tab[tab.size() - 1].typ = nul;
        tab[tab.size() - 1].ref = btab.size() - 1;

        test(set<Symbol>() = { LPAREN, COLON, SEMICOLON }, fsys, 5);

        if (sym == LPAREN && level > 1) {
            parameterlist(fsys);
            // incase there is an empty par-list
            if (tab[tab.size() - 1].obj == objvariable) {
                btab[btab.size() - 1].last = tab.size() - 1;
                btab[btab.size() - 1].lpar = tab.size() - 1;
            }
            int ps = 0;
            int i = tab.size() - 1;
            for (; i != 0; i = tab[i].link) ps += 4;
            btab[btab.size() - 1].psize = ps;
        }

        // parameter list mem alloc: -4 -8 -12
        int mem = 0;
        for (int i = btab[btab.size() - 1].lpar; i != 0; i = tab[i].link) {
            // assert(tab[i].obj == objvariable);
            mem -= 4; tab[i].adr = mem;
        }

        if (isFunc) {
            if (sym == COLON) {
                getsym();
                if (sym == INTEGER || sym == CHAR) {
                    tab[index].typ = sym == INTEGER ? integer : character;
                    getsym();
                }
                else {
                    error(13);
                    temp = { SEMICOLON };
                    temp.insert(fsys.begin(), fsys.end());
                    while (temp.find(sym) == temp.end()) getsym();
                }
            }
            else error(14);
        }

        if (sym == SEMICOLON) getsym();
        else error(5);
    }

    temp = blockbegsys;
    temp.insert(statebegsys.begin(), statebegsys.end());
    do {
        
        if (sym == CONSTSYM) constdeclaration(fsys);
        if (sym == VARSYM)   vardeclaration(fsys);
        if (sym == CONSTSYM)
            test(set<Symbol>() = {PROCSYM, FUNSYM, BEGINSYM }, temp, 48);
        // @TO-DO record memory address
        while (sym == PROCSYM || sym == FUNSYM) procdeclaration(fsys);
        if (sym == BEGINSYM) info("compound statement");
        test(set<Symbol>() = { BEGINSYM }, temp, 0);
    } while (statebegsys.find(sym) == statebegsys.end());

    if (level == 1) {
        Operand entry;
        entry.type = olabel;
        entry.label = "entry";
        instrs.push_back(Instruction(LABEL, entry));
    }
    else instrs.push_back(Instruction(LABEL, lab));

    getsym();
    temp = { SEMICOLON, ENDSYM };
    temp.insert(fsys.begin(), fsys.end());
    statement(temp);

    while (sym == SEMICOLON || statebegsys.find(sym) != statebegsys.end()) {
        if (sym == SEMICOLON) getsym();
        else error(5);
        statement(temp);
    }

    if (sym == ENDSYM) getsym();
    else error(26);

    if (level == 1) {
        temp = { PERIOD };
        temp.insert(fsys.begin(), fsys.end());
        test(temp, set<Symbol>(), 7);
    }

    level--;
}

void test(set<Symbol> s1, set<Symbol> s2, int errId) {
    if (s1.find(sym) != s1.end()) return;

    error(errId);
    s1.insert(s2.begin(), s2.end());
    while (s1.find(sym) == s1.end()) getsym();
}

void testsemicolon(set<Symbol> legal, set<Symbol> fsys) {
    if (sym == SEMICOLON) getsym();
    else {
        error(5);
        if (sym == COMMA || sym == COLON) getsym();
    }
    test(legal, fsys, 7);
}

ConstRec constant(set<Symbol> fsys) {
    ConstRec constrec;
    constrec.type = nul;
    constrec.value = 0;

    test(constbegsys, fsys, 2);

    // PLUS, MINUS, CONSTCHAR, NUMBER
    if (constbegsys.find(sym) != constbegsys.end()) {
        if (sym == PLUS || sym == MINUS) {
            int sign = (sym == MINUS) ? -1 : 1;
            getsym();
            if (sym != NUMBER) error(2);
            else {
                constrec.type = integer;
                constrec.value = sign * number;
                getsym();
            }
        }
        else if (sym == NUMBER) {
            constrec.type = integer;
            constrec.value = number;
            getsym();
        }
        else if (sym == CONSTCHAR) {
            char ch = word[0];
            if (word.length() != 1) error(9);
            if (!isdigit(ch) && !isalpha(ch)) error(11);
            else {
                constrec.type = character;
                constrec.value = ch;
                getsym();
            }
        }
        else error(2);
        set<Symbol> temp = { COLON };
        temp.insert(fsys.begin(), fsys.end());
        test(temp, set<Symbol>(), 7);
    }
    return constrec;
}

void parameterlist(set<Symbol> fsys) {
    bool valpar = true;
    int index;
    set<Symbol> temp = { RPAREN };
    temp.insert(fsys.begin(), fsys.end());

    getsym();
    test(set<Symbol>() = { IDENT, VARSYM }, temp, 12);

    while (sym == IDENT || sym == VARSYM) {
        if (sym == VARSYM) {
            getsym();
            valpar = false;
        }
        else valpar = true;

        index = tab.size();

        if (sym == IDENT) {
            if (loc(id, false) != 0) error(36);
            else enter(id, objvariable, nul, 0);
            getsym();
        }
        else error(3);

        while (sym == COMMA) {
            getsym();
            if (sym == IDENT) {
                if (loc(id, false) != 0) error(36);
                else enter(id, objvariable, nul, 0);
                getsym();
            }
            else error(3);
        }

        TokenType type = nul;
        if (sym == COLON) {
            getsym();
            if (sym == CHAR || sym == INTEGER) {
                type = sym == INTEGER ? integer : character;
                getsym();
            }
            else error(13);

            temp = { COMMA, IDENT };
            temp.insert(fsys.begin(), fsys.end());
            test(set<Symbol>() = { SEMICOLON, RPAREN }, temp, 5);
        }
        else error(14);

        while (index < tab.size()) {
            tab[index].typ = type;
            tab[index].normal = valpar;
            index++;
        }

        if (sym != RPAREN) {
            if (sym == SEMICOLON) getsym();
            else {
                error(5);
                if (sym == COMMA) getsym();
            }

            temp = { RPAREN };
            temp.insert(fsys.begin(), fsys.end());
            test(set<Symbol>() = { IDENT, VARSYM }, fsys, 7);
        }
    }

    if (sym == RPAREN) {
        getsym();
        test(set<Symbol>() = { SEMICOLON, COLON }, fsys, 7);
    }
    else error(15);
}

void constdeclaration(set<Symbol> fsys) {
    info("const declaration");

    set<Symbol> temp = { SEMICOLON, COMMA };
    temp.insert(fsys.begin(), fsys.end());

    int l = 0;
    getsym();
    test(set<Symbol>() = { IDENT }, blockbegsys, 3);
    while (sym == IDENT) {
        l = loc(id, false);
        if (l != 0) error(36);
        else enter(id, objconstant, nul, 0);
        getsym();
        if (sym == EQL) getsym();
        else {
            error(1);
            if (sym == BECOMES) getsym();
        }

        ConstRec cr = constant(temp);
        if (l == 0) {
            tab[tab.size() - 1].typ = cr.type;
            tab[tab.size() - 1].adr = cr.value;
        }

        while (sym == COMMA) {
            getsym();
            if (sym == IDENT) {
                l = loc(id, false);
                if (l != 0) error(36);
                else enter(id, objconstant, nul, 0);
                getsym();
                if (sym == EQL) getsym();
                else {
                    error(1);
                    if (sym == BECOMES) getsym();
                }

                ConstRec cr = constant(temp);
                if (l == 0) {
                    tab[tab.size() - 1].typ = cr.type;
                    tab[tab.size() - 1].adr = cr.value;
                }
            }
            else error(3);
        }

        if (sym == IDENT) error(16);
    }

    // set<Symbol> temp = { VARSYM, PROCSYM, FUNSYM, BEGINSYM };
    testsemicolon(blockbegsys, fsys);
}

void vardeclaration(set<Symbol> fsys) {
    info("var declaration");
    int t0, t1, arrsize;
    getsym();

    test(set<Symbol>() = { IDENT }, fsys, 7);
    while (sym == IDENT) {
        t0 = tab.size();
        if (loc(id, false) != 0) error(36);
        else enter(id, objvariable, nul, 0);
        getsym();

        while (sym == COMMA) {
            getsym();
            if (sym == IDENT) {
                if (loc(id, false) != 0) error(36);
                else enter(id, objvariable, nul, 0);
                getsym();
            }
            else error(3);
        }

        if (sym == COLON) getsym();
        else error(14);

        t1 = tab.size();

        if (sym == ARRAY) {
            getsym();
            if (sym == LBRACKET) getsym();
            else {
                error(18);
                if (sym == LPAREN) getsym();
            }

            if (sym == NUMBER) {
                arrsize = number;
                getsym();
            }
            else {
                error(19);
                getsym();
            }

            if (sym == RBRACKET) getsym();
            else {
                error(20);
                if (sym == RPAREN) getsym();
            }

            if (sym == OFSYM) getsym();
            else error(21);

            if (sym == INTEGER || sym == CHAR) {
                for (int i = t0; i < t1; i++) {
                    tab[i].typ = arrays;
                    ArrayTableItem item;
                    item.length = arrsize;
                    item.type = sym == INTEGER ? integer : character;
                    atab.push_back(item);
                    tab[i].ref = atab.size() - 1;
                    tab[i].adr = mempoint[level];
                    mempoint[level] += UNIT * atab[tab[i].ref].length;
                }
                getsym();
            }
            else error(22);
        }
        else if (sym == INTEGER || sym == CHAR) {
            for (int i = t0; i < t1; i++) {
                tab[i].typ = sym == INTEGER ? integer : character;
                tab[i].adr = mempoint[level];
                mempoint[level] += UNIT;
            }
            getsym();
        }
        else error(17);

        set<Symbol> temp = { FUNSYM, PROCSYM, BEGINSYM };
        temp.insert(IDENT);
        testsemicolon(temp, fsys);
    }
}

void procdeclaration(set<Symbol> fsys) {
    bool isfun = sym == FUNSYM;
    if (isfun) info("function declaration");
    else       info("procedure declaration");

    getsym();
    if (sym != IDENT) {
        error(3);
        id = "          ";
    }

    if (loc(id, false) != 0) error(36);
    else if (isfun) enter(id, objfunction, nul, 0);
    else            enter(id, objprocedure, nul, 0);

    Operand ret;
    ret.type = oident;
    ret.ident = loc(id, false);

    Operand lab;
    lab.type = olabel;
    stringstream ss;
    ss << "label_" << label++;
    lab.label = ss.str();
    // instrs.push_back(Instruction(LABEL, lab));
    ptab.push_back(lab.label);
    tab[tab.size() - 1].adr = ptab.size() - 1;

    getsym();
    set<Symbol> temp = { SEMICOLON };
    temp.insert(fsys.begin(), fsys.end());
    block(blockbegsys, isfun, lab);

    if (sym == SEMICOLON) getsym();
    else error(5);

    instrs.push_back(Instruction(RET, ret));
}

void statement(set<Symbol> fsys) {
    int l = 0;
    Operand res;

    // if sym is valid
    if (sym == IDENT || statebegsys.find(sym) != statebegsys.end()) {
        switch (sym) {
        case IDENT:
            l = loc(id);
            if (l == 0) error(27);
            getsym();
            if (sym == EQL || sym == BECOMES || sym == LBRACKET)
                assignment(fsys);
            else if (sym == LPAREN || sym == ENDSYM || sym == SEMICOLON)
                callstatement(fsys, l, res);
            break;
        case BEGINSYM:
            compoundstatement(fsys);
            break;
        case IFSYM:
            ifstatement(fsys);
            break;
        case CASESYM:
            casestatement(fsys);
            break;
        case FORSYM:
            forstatement(fsys);
            break;
        case READSYM:
            readstatement(fsys);
            break;
        case WRITESYM:
            writestatement(fsys);
            break;
        }
    }
    test(fsys, set<Symbol>(), 5);
}

void callstatement(set<Symbol> fsys, int l, Operand &res) {
    info("call statement");

    instrs.push_back(Instruction(SAVE));

    set<Symbol> temp = { COMMA, COLON, RPAREN };
    temp.insert(fsys.begin(), fsys.end());

    Operand pro;
    pro.type = oident;
    pro.ident = l;

    int lastp = btab[tab[l].ref].lpar;
    int curp = l;
    int k;
    int count = 1;

    if (sym == LPAREN) {
        // incase there is no need for parameters
        if (lastp == 0) {
            error(45);
            test(set<Symbol>() = { RPAREN }, fsys, 7);
        }
        else do {
            Operand o;
            getsym();
            if (curp >= lastp) error(40);
            else {
                curp++;
                if (tab[curp].normal) { // valpar
                    TokenType type = expression(temp, o);
                    if (type != tab[curp].typ) {
                        if (type > tab[curp].typ) error(41);
                        else;
                    }
                    else;
                }
                else { // varpar
                    if (sym != IDENT) error(3);
                    else {
                        k = loc(id);
                        getsym();
                        o.type = oident;
                        o.ident = k;
                        if (k == 0) error(27);
                        else {
                            if (tab[k].obj != objvariable) error(42);
                            if (tab[k].normal); // gen
                            else; // gen
                            if (tab[k].typ == arrays) {
                                if (atab[tab[k].ref].type != tab[curp].typ) error(41);
                                if (sym != LBRACKET) error(18);
                                else {
                                    fsys.insert(COMMA);
                                    Operand off, res;
                                    selector(fsys, off);
                                    // check index overflow
                                    if (off.type == oconst) {
                                        if (off.constant < 0 || off.constant >= atab[tab[k].ref].length)
                                            error(39);
                                    }
                                    fsys.erase(COMMA);
                                    res.type = otemp;
                                    stringstream ss;
                                    ss << "#" << tempi++;
                                    res.label = ss.str();
                                    enter(ss.str(), objvariable, atab[tab[k].ref].type, mempoint[level]);
                                    mempoint[level] += 4; // alloc mem
                                    res.ident = tab.size() - 1;
                                    instrs.push_back(Instruction(OFF, res, o, off));
                                    o = res;
                                }
                            }
                            else {
                                if (tab[k].typ != tab[curp].typ) error(41);
                                else;
                            }
                        }
                    }
                }
            }
            test(set<Symbol>() = { COMMA, RPAREN }, fsys, 7);
            Operand cnt;
            cnt.type = oconst;
            cnt.constant = count++;
            instrs.push_back(Instruction(PUSH, pro, cnt, o));
        } while (sym == COMMA);
        if (sym == RPAREN) getsym();
        else error(15);
    }

    if (tab[l].obj == objfunction) {
        res.type = otemp;
        stringstream ss;
        ss << "#" << tempi++;
        res.label = ss.str();
        enter(ss.str(), objvariable, tab[l].typ, mempoint[level]);
        mempoint[level] += 4; // alloc mem
        res.ident = tab.size() - 1;
        pro.type = olabel;
        pro.label = ptab[tab[l].adr];
        instrs.push_back(Instruction(CALF, pro, res));
    }
    else if (tab[l].obj == objprocedure) {
        pro.type = olabel;
        pro.label = ptab[tab[l].adr];
        instrs.push_back(Instruction(CALP, pro));
    }

}

void assignment(set<Symbol> fsys) {
    info("assignment statement");

    bool isfunc, isarray;

    Operand left;
    left.type = oident;
    left.ident = loc(id);

    isfunc = tab[loc(id)].obj == objfunction;
    isarray = tab[loc(id)].typ == arrays;

    if (tab[loc(id)].obj == objconstant) error(46);

    set<Symbol> temp = { BECOMES, EQL };
    temp.insert(fsys.begin(), fsys.end());

    if (sym == LBRACKET || sym == LPAREN) {
        Operand off, o;
        selector(temp, off);
        // check index overflow
        if (off.type == oconst) {
            if (off.constant < 0 || off.constant >= atab[tab[left.ident].ref].length)
                error(39);
        }
        o.type = otemp;
        stringstream ss;
        ss << "#" << tempi++;
        o.label = ss.str();
        enter(ss.str(), objvariable, tab[loc(id)].typ, mempoint[level], 0, false); // this false
        mempoint[level] += 4;
        o.ident = tab.size() - 1;
        instrs.push_back(Instruction(OFF, o, left, off));
        left = o;
    }

    if (sym == BECOMES) getsym();
    else {
        error(28);
        if (sym == EQL) getsym();
    }

    Operand right;
    expression(fsys, right);
    if (isfunc) instrs.push_back(Instruction(RVAL, right));
    else        instrs.push_back(Instruction(ASG, left, right));
}

void compoundstatement(set<Symbol> fsys) {
    info("compound statement");

    set<Symbol> temp = { SEMICOLON, ENDSYM };
    temp.insert(fsys.begin(), fsys.end());

    getsym();
    statement(temp);
    while (sym == SEMICOLON || statebegsys.find(sym) != statebegsys.end()) {
        if (sym == SEMICOLON) getsym();
        else error(5);
        statement(temp);
    }
    if (sym == ENDSYM) getsym();
    else error(6);
}

void ifstatement(set<Symbol> fsys) {
    info("if statement");

    set<Symbol> temp = { THENSYM, DOSYM };
    temp.insert(fsys.begin(), fsys.end());

    getsym();

    Operand res, end;

    condition(temp, res);
    if (sym == THENSYM) getsym();
    else {
        error(23);
        if (sym == DOSYM) getsym();
    }

    temp = { ELSESYM };
    temp.insert(fsys.begin(), fsys.end());
    statement(temp);

    bool haveelse = sym == ELSESYM;
    if (haveelse) {
        end.type = olabel;
        stringstream ss;
        ss << "label_" << label++;
        end.label = ss.str();
        instrs.push_back(Instruction(JMP, end));
    }
    instrs.push_back(Instruction(LABEL, res));

    if (haveelse) {
        getsym();
        statement(fsys);
        instrs.push_back(Instruction(LABEL, end));
    }

}

void casestatement(set<Symbol> fsys) {
    info("case statement");

    Operand obj, end;
    end.type = olabel;
    stringstream ss;
    ss << "label_" << label++;
    end.label = ss.str();

    set<Symbol> temp = { OFSYM, COMMA, COLON };
    temp.insert(fsys.begin(), fsys.end());

    getsym();
    TokenType type = expression(temp, obj);

    if (type != integer && type != character) error(25);

    if (sym == OFSYM) getsym();
    else error(21);

    onecase(fsys, type, obj, end);
    while (sym == SEMICOLON) {
        getsym();
        onecase(fsys, type, obj, end);
    }
    if (sym == ENDSYM) getsym();
    else error(26);
    instrs.push_back(Instruction(LABEL, end));
}

void caselabel(set<Symbol> fsys, TokenType type, Operand &res) {
    set<Symbol> temp = { COMMA, COLON };
    temp.insert(fsys.begin(), fsys.end());
    ConstRec cr = constant(fsys);

    if (cr.type != type) error(24);
    else;

    res.type = oconst;
    res.constant = cr.value;
}

void onecase(set<Symbol> fsys, TokenType type, Operand obj, Operand end) {
    test(constbegsys, fsys, 37);
    if (constbegsys.find(sym) != constbegsys.end()) {
        Operand next;
        next.type = olabel;
        stringstream ss;
        ss << "label_" << label++;
        next.label = ss.str();

        Operand c;
        caselabel(fsys, type, c);
        instrs.push_back(Instruction(BNE, next, obj, c));

        if (sym == COLON) getsym();
        else error(14);
        set<Symbol> temp = { SEMICOLON, ENDSYM };
        temp.insert(fsys.begin(), fsys.end());
        statement(temp);

        instrs.push_back(Instruction(JMP, end));
        instrs.push_back(Instruction(LABEL, next));
    }
}

void forstatement(set<Symbol> fsys) {
    info("for statement");

    int step = 1;
    Operand itr, ori, des, stp;
    Operand start, end;
    start.type = olabel;
    stringstream ss1;
    ss1 << "label_" << label++;
    start.label = ss1.str();
    end.type = olabel;
    stringstream ss2;
    ss2 << "label_" << label++;
    end.label = ss2.str();

    set<Symbol> temp = { BECOMES, TOSYM, DOWNTOSYM, DOSYM };
    temp.insert(fsys.begin(), fsys.end());

    getsym();
    if (sym == IDENT) {
        int l = loc(id);
        if (l == 0) error(27);
        itr.type = oident;
        itr.ident = l;
        getsym();
    }
    else {
        error(3);
        while (temp.find(sym) == temp.end()) getsym();
    }

    if (sym == BECOMES) {
        getsym();
        temp.erase(BECOMES);
        expression(temp, ori);
    }
    else {
        error(28);
        while (temp.find(sym) == temp.end()) getsym();
    }
    instrs.push_back(Instruction(ASG, itr, ori));
    instrs.push_back(Instruction(LABEL, start));

    if (sym == TOSYM || sym == DOWNTOSYM) {
        step = sym == TOSYM ? 1 : -1;
        getsym();
        temp.erase(TOSYM); temp.erase(DOWNTOSYM);
        expression(temp, des);
    }
    else {
        error(29);
        while (temp.find(sym) == temp.end()) getsym();
    }
    if (step > 0) instrs.push_back(Instruction(BGT, end, itr, des));
    else          instrs.push_back(Instruction(BLT, end, itr, des));

    if (sym == DOSYM) getsym();
    else error(30);

    statement(fsys);
    stp.type = oconst;
    stp.constant = step;
    instrs.push_back(Instruction(ADD, itr, itr, stp));
    instrs.push_back(Instruction(JMP, start));
    instrs.push_back(Instruction(LABEL, end));
}

void readstatement(set<Symbol> fsys) {
    info("read statement");

    getsym();
    if (sym == LPAREN) {
        do {
            getsym();
            if (sym != IDENT) error(3);
            else {
                int l = loc(id);
                if (l == 0) error(27);
                Operand res;
                res.type = oident;
                res.ident = l;
                instrs.push_back(Instruction(READ, res));
                getsym();
            }
            test(set<Symbol>() = { COMMA, RPAREN }, fsys, 7);
        } while (sym == COMMA);
        if (sym == RPAREN) getsym();
        else error(15);
    }
    else error(31);
}

void writestatement(set<Symbol> fsys) {
    info("write statement");

    set<Symbol> temp = { RBRACKET };
    temp.insert(fsys.begin(), fsys.end());
    getsym();
    if (sym == LPAREN) {
        getsym();
        if (sym == PLUS || sym == MINUS || facbegset.find(sym) != facbegset.end()) {
            Operand res;
            expression(fsys, res);
            instrs.push_back(Instruction(WRITE, res));
        }
        else if (sym == CONSTSTR) {
            if (word.at(word.length() - 1) == '\n') error(10);
            if (!isStrLegal(word)) error(35);
            if (word[word.size() - 1] == '\n') word = word.substr(0, word.size() - 1);
            stab.push_back("\"" + word + "\"");

            Operand res;
            res.type = ostring;
            res.str = stab.size() - 1;
            instrs.push_back(Instruction(WRITE, res));

            getsym();
            if (sym == COMMA) {
                getsym();
            }
            else if (sym != RPAREN) {
                error(32);
            }

            if (sym != RPAREN) {
                if (sym == PLUS || sym == MINUS || facbegset.find(sym) != facbegset.end()) {
                    Operand res;
                    expression(temp, res);
                    instrs.push_back(Instruction(WRITE, res));
                }
            }
        }
        else error(31);
        if (sym == RPAREN) getsym();
        else error(15);
    }
    else error(31);
}

void condition(set<Symbol> fsys, Operand &res) {
    Symbol op;
    set<Symbol> temp = { LSS, LEQ, GTR, GEQ, NEQ, EQL };
    temp.insert(fsys.begin(), fsys.end());
    Operand o1, o2;
    expression(temp, o1);

    if (sym == LSS || sym == LEQ || sym == GTR ||
        sym == GEQ || sym == NEQ || sym == EQL) {
        op = sym;
        getsym();
        expression(temp, o2);
    }
    else error(34);

    res.type = olabel;
    stringstream ss;
    ss << "label_" << label++;
    res.label = ss.str();
    switch (op) {
    case LSS: // <
        instrs.push_back(Instruction(BGE, res, o1, o2));
        break;
    case LEQ: // <=
        instrs.push_back(Instruction(BGT, res, o1, o2));
        break;
    case GTR: // >
        instrs.push_back(Instruction(BLE, res, o1, o2));
        break;
    case GEQ: // >=
        instrs.push_back(Instruction(BLT, res, o1, o2));
        break;
    case NEQ: // <>
        instrs.push_back(Instruction(BEQ, res, o1, o2));
        break;
    case EQL: // ==
        instrs.push_back(Instruction(BNE, res, o1, o2));
        break;

    }
}

TokenType expression(set<Symbol> fsys, Operand &res) {
    TokenType t1 = nul, t2 = nul;
    Symbol op = NUL;
    Operand o1, o2;

    set<Symbol> temp = { PLUS, MINUS };
    temp.insert(fsys.begin(), fsys.end());

    if (sym == PLUS || sym == MINUS) {
        op = sym;
        getsym();
        t1 = term(temp, o1);

        if (op == MINUS) {
            Operand neg;
            neg.type = otemp;
            stringstream ss;
            ss << "#" << tempi++;
            neg.label = ss.str();
            enter(ss.str(), objvariable, integer, mempoint[level]);
            mempoint[level] += 4;
            neg.ident = tab.size() - 1;
            instrs.push_back(Instruction(NEG, neg, o1));
            o1 = neg;
        }
    }
    else t1 = term(temp, o1);

    res = o1;

    Operand o;
    while (sym == PLUS || sym == MINUS) {
        op = sym;
        getsym();
        t2 = term(temp, o2);
        t1 = resulttype(t1, t2);
        o.type = otemp;
        stringstream ss;
        ss << "#" << tempi++;
        o.label = ss.str();
        enter(ss.str(), objvariable, t1, mempoint[level]);
        mempoint[level] += 4;
        o.ident = tab.size() - 1;
        if (op == PLUS) instrs.push_back(Instruction(ADD, o, o1, o2));
        else            instrs.push_back(Instruction(SUB, o, o1, o2));
        o1 = o;
    }

    res = o1;
    return t1;
}

TokenType selector(set<Symbol> fsys, Operand &res) {
    if (sym != LBRACKET) error(18);
    getsym();

    set<Symbol> temp = { RBRACKET };
    temp.insert(fsys.begin(), fsys.end());
    TokenType type = expression(fsys, res);

    if (sym == RBRACKET) getsym();
    else {
        error(20);
        if (sym == RPAREN) getsym();
    }
    temp = { RPAREN, RBRACKET };
    temp.insert(fsys.begin(), fsys.end());
    test(temp, set<Symbol>(), 7);
    return type;
}

TokenType term(set<Symbol> fsys, Operand &res) {
    TokenType t1 = nul, t2 = nul;
    Symbol op = NUL;
    Operand o1, o2;

    set<Symbol> temp = { TIMES, SLASH };
    temp.insert(fsys.begin(), fsys.end());
    t1 = factor(temp, o1);

    res = o1;

    Operand o;
    while (sym == TIMES || sym == SLASH) {
        op = sym;
        getsym();
        t2 = factor(temp, o2);
        t1 = resulttype(t1, t2);

        o.type = otemp;
        stringstream ss;
        ss << "#" << tempi++;
        o.label = ss.str();
        enter(ss.str(), objvariable, t1, mempoint[level]);
        mempoint[level] += 4;
        o.ident = tab.size() - 1;
        if (op == TIMES) instrs.push_back(Instruction(MUL, o, o1, o2));
        else             instrs.push_back(Instruction(DIV, o, o1, o2));
        o1 = o;
    }

    res = o1;
    return t1;
}

TokenType factor(set<Symbol> fsys, Operand &res) {
    TokenType type = nul;
    test(facbegset, fsys, 33);

    while (facbegset.find(sym) != facbegset.end()) {
        if (sym == IDENT) {
            int l = loc(id);
            if (l == 0) error(27);
            res.type = oident;
            res.ident = l;
            getsym();
            switch (tab[l].obj) {
            case objconstant:
                type = tab[l].typ;
                break;
            case objvariable:
                type = tab[l].typ;
                if (type == arrays) type = atab[tab[l].ref].type;
                if (sym == LBRACKET) {
                    Operand off, o;
                    selector(fsys, off);
                    // check index overflow
                    if (off.type == oconst) {
                        if (off.constant < 0 || off.constant >= atab[tab[l].ref].length)
                            error(39);
                    }
                    o.type = otemp;
                    stringstream ss;
                    ss << "#" << tempi++;
                    o.label = ss.str();
                    enter(ss.str(), objvariable, type, mempoint[level]);
                    mempoint[level] += 4;
                    o.ident = tab.size() - 1;
                    instrs.push_back(Instruction(SEL, o, res, off));
                    res = o;
                }
                break;
            case objfunction:
                type = tab[l].typ;
                if (tab[l].lev != 0) callstatement(fsys, l, res);
                break;
            case objprocedure:
                // incase didn't find the ident
                if (l != 0) error(43);
                break;
            }
        }
        else if (sym == NUMBER) {
            type = integer;
            res.type = oconst;
            res.constant = number;
            getsym();
        }
        else if (sym == LPAREN) {
            getsym();
            set<Symbol> temp = { RPAREN };
            temp.insert(fsys.begin(), fsys.end());
            type = expression(temp, res);
            if (sym == RPAREN) getsym();
            else error(15);
        }
        set<Symbol> temp = { RPAREN, RBRACKET };
        temp.insert(fsys.begin(), fsys.end());
        test(temp, facbegset, 7);
    }

    return type;
}

TokenType resulttype(TokenType type1, TokenType type2) {
    return integer;
    //if (type1 == type2 && type1 == character) return integer;
    //else return type1 > type2 ? type1 : type2; // integer > char
}

int loc(string id, bool iswhole) {
    int i = level;
    int j = 0;

    tab[0].name = id;
    do {
        j = btab[display[i]].last;
        while (tab[j].name != id) j = tab[j].link;
        if (iswhole) i--;
        else break;
    } while (i >= 0 && j == 0);

    return j;
}

void info(string msg) {
    string s = "";
    for (int i = 1; i < level; i++)
        s += '\t';
    cout << s + msg << endl;
}

bool isStrLegal(string str) {
    for (int i = 0; i < str.length(); i++) {
        char ch = str[i];
        if (ch != 32 && ch != 33 && (ch < 35 || ch > 126)) return false;
    }
    return true;
}