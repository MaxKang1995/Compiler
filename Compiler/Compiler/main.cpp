#include "Parser.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    string s;
    cout << "Input test file path:";
    getline(cin, s);
    parser(s);
    //parser("sample.txt");
    //parser("source.txt");
	system("pause");
}