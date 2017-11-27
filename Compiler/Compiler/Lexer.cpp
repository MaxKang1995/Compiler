#include "Symbol.h"
#include "Lexer.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#define MAX_LINE_NUM 1000

using namespace std;

extern ifstream src;

string id = "";
string word = "";
Symbol sym = NUL;
char ch = '\0';
int ichar = 0;
int number = 0;
int line = 1;

void getsym() {
    // ¹ýÂË¿Õ°×·û
    while (ch == ' ' || ch == '\t' || ch == '\n') {
        if (ch == '\n') { line++; ichar = 0; }
        getch();
    }

    if (isdigit(ch)) { // ¶Áµ½µÄ×Ö·ûÊÇÊý×Ö
        number = 0;
        sym = NUMBER;
        word = "";
        while (isdigit(ch)) { word += ch; getch(); }
        if (word.length() > MAX_LENGTH_NUMBER) {
            word.substr(0, MAX_LENGTH_NUMBER); error(47);
        }
        for (int i = 0; i < word.length(); i++) number = number * 10 + word[i] - '0';
    }
    else if (isalpha(ch)) { // ¶Áµ½µÄ×Ö·ûÊÇ×ÖÄ¸
        word = "";
        while (isdigit(ch) || isalpha(ch)) { word += ch; getch(); }
        if (word.length() > MAX_LENGTH_IDENT) {
            word.substr(0, MAX_LENGTH_IDENT); error(44);
        }
        else for (int i = word.length(); i < MAX_LENGTH_IDENT; i++) word += " ";
        string lowercase = word;
        transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
        map<string, Symbol>::const_iterator finder = str2sym.find(lowercase);
        if (finder != str2sym.end()) sym = finder->second;
        else { sym = IDENT; id = lowercase; }
    }
    else if (ch == ':') { // :  :=
        getch();
        if (ch == '=') {
            word = ":=";
            sym = BECOMES;
            getch();
        }
        else {
            word = ":";
            sym = COLON;
        }
    }
    else if (ch == '<') { // <  <=  <>
        getch();
        if (ch == '=') {
            word = "<=";
            sym = LEQ;
            getch();
        }
        else if (ch == '>') {
            word = "<>";
            sym = NEQ;
            getch();
        }
        else {
            word = "<";
            sym = LSS;
        }
    }
    else if (ch == '>') { // >  >=
        getch();
        if (ch == '=') {
            word = ">=";
            sym = GEQ;
            getch();
        }
        else {
            word = ">";
            sym = GTR;
        }
    }
    else if (ch == '"') { // " ... "
        word = "";
        getch();
        sym = CONSTSTR;
        while (ch != '"') {
            word += ch;
            if (ch == '\n') return;
            getch();
        }
        getch();
    }
    else if (ch == '\'') { // 'a'
        word = "";
        getch();
        sym = CONSTCHAR;
        word = ch;
        getch();
        if (ch != '\'') word += '\'';
        else getch();
    }
    else if (ch == '.') { // .
        word = ".";
        sym = PERIOD;
    }
    else {
        word = ch;
        map<char, Symbol>::const_iterator finder = char2sym.find(ch);
        if (finder != char2sym.end()) { sym = finder->second; }
        else sym = NUL;
        getch();
    }
}

void getch()
{
    if (src.eof()) { src.close(); fatal(3); }
    ch = src.get();
    ichar++;
}