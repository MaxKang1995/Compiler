#include "Error.h"
#include <iostream>

using namespace std;

extern int ichar;
extern int line;

int errnum = 0;

const string errorlist[] = {
    /*0*/  "期望一个 begin",
    /*1*/  "期望一个 =",
    /*2*/  "常量声明中只能赋值整数和字符常量",
    /*3*/  "期望一个标识符",
    /*4*/  "const var procedure function后面应为标识符",
    /*5*/  "期望一个 ;",
    /*6*/  "期望一个 end",
    /*7*/  "非法字符，编译程序将跳读",
    /*8*/  "错误的字符常量",
    /*9*/  "期望一个 '",
    /*10*/ "期望一个 \"",
    /*11*/ "错误的常量字符",
    /*12*/ "参数表应该由 'var' 或一个标识符开始",
    /*13*/ "参数只能是integer和char类型",
    /*14*/ "期望一个 :",
    /*15*/ "期望一个 )",
    /*16*/ "期望一个 ,",
    /*17*/ "类型错误",
    /*18*/ "期望一个 [",
    /*19*/ "数组大小应为无符号整数",
    /*20*/ "期望一个 ]",
    /*21*/ "期望一个 of",
    /*22*/ "数组元素的类型只能为integer和char类型",
    /*23*/ "期望一个 then",
    /*24*/ "case lable类型和case表达式类型不一致",
    /*25*/ "case的表达式的类型应该为integer或者char",
    /*26*/ "期望一个 end",
    /*27*/ "未定义的标识符",
    /*28*/ "期望一个 :=",
    /*29*/ "期望一个 to 或 downto",
    /*30*/ "期望一个 do",
    /*31*/ "无参数",
    /*32*/ "期望一个 , 或 ;",
    /*33*/ "factor应由一个标识符，一个无符号整数或者一个'('开头",
    /*34*/ "期望一个关系操作符",
    /*35*/ "字符串中有非法字符",
    /*36*/ "标识符重复定义",
    /*37*/ "情况语句中每一种情况应该以一个常量开始",
    /*38*/ "期望一个 .",
    /*39*/ "数组下标越界",
    /*40*/ "实参个数和形参个数不等",
    /*41*/ "实参和形参的类型不匹配",
    /*42*/ "应是变量",
    /*43*/ "不可以是过程名",
    /*44*/ "标识符太长",
    /*45*/ "过程没有形参，无需使用实参列表",
    /*46*/ "不能给常量赋值",
    /*47*/ "数字太长",
    /*48*/ "const声明必须在var声明之前",
};

const string fatallist[] = {
    /*0*/  "层数过大",
    /*1*/  "未找到源程序文件",
    /*2*/  "源程序有错误，编译结束",
    /*3*/  "源文件不完整",
};

void error(int id) {
    cout << "error : line " << line << " index " << ichar << " --- " << errorlist[id] << endl;
    errnum++;
}

void fatal(int id) {
    cout << "fatal : " << fatallist[id] << endl;
    system("pause");
    exit(-1);
}