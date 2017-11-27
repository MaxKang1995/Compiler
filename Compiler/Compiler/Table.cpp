#include "Table.h"
#include "Error.h"
#include <vector>
#include <assert.h>
#include <iostream>

extern int level;
extern string id;

vector<TokenTableItem> tab;   // index : table.size()
vector<ArrayTableItem> atab;  // index : atab.size() - 1
vector<BlockTableItem> btab;  // index : btab.size() - 1
vector<string>         stab;  // index : stab.size() - 1
vector<string>         ptab;  
int display[MAX_LEVEL + 1];   // index : level

void initTable() {
    TokenTableItem item;
    item.name = "          ";
    item.link = 0;
    item.adr = 0;
    item.lev = level;
    item.normal = true;
    item.obj = objprocedure;
    item.typ = nul;
    item.ref = 0;
    tab.push_back(item);
}

// standard token
void enter(string name, TokenObject obj, TokenType typ,
    int adr, int ref, bool normal) {
    int j, l;

    TokenTableItem item;
    tab[0].name = name;
    j = btab[display[level]].last;
    l = j;

    while (tab[j].name != name) {
        j = tab[j].link;
    }
    if (j != 0) error(36);
    else {
        item.name = name;
        item.link = l;
        item.obj = obj;
        item.typ = typ;
        item.ref = 0;
        item.adr = adr;
        item.lev = level;
        item.normal = normal;
        tab.push_back(item);
        btab[display[level]].last = tab.size() - 1;
    }
}

void printtables() {
    tab[0].name = "PROGRAM   ";

    // tab
    cout << endl;
    cout << "=== TOKEN TABLE ===" << endl;
    cout << "no" << "\t" << "name      " << "  " << "lev" << "  " << "link" << "\t"
         << "obj       " << " " << "typ       " << "ref   " << "adr\t" << "normal" << endl;
    for (int i = 0; i < tab.size(); i++)
        cout << i << "\t" << tab[i].name << "  " << tab[i].lev << "    " << tab[i].link 
             << "\t" << to2str[tab[i].obj] << " " << tt2str[tab[i].typ] << tab[i].ref 
             << "     " << tab[i].adr << "\t" << tab[i].normal << endl;

    // btab
    cout << endl;
    cout << "=== BLOCK TABLE ===" << endl;
    cout << "no" << "\t" << "last" << "\t" << "lpar" << "\t" << "psize" << endl;
    for (int i = 0; i < btab.size(); i++)
        cout << i << "\t" << btab[i].last << "\t" << btab[i].lpar << "\t" 
             << btab[i].psize << endl;

    // atab
    cout << endl;
    cout << "=== ARRAY TABLE ===" << endl;
    cout << "no" << "\t" << "type" << endl;
    for (int i = 0; i < atab.size(); i++)
        cout << i << "\t" << tt2str[atab[i].type] << endl;
}