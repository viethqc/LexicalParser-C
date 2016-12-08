#include <iostream>
#include <stdio.h>
#include "Parser.h"
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

int main() {
    CParser parser;

    ifstream t("input.txt");
    string strInput((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    if (parser.CheckGrammar(strInput) == true)
    {
        printf("Van pham hop le\n");
    }
    else
    {
        printf("Van pham khong hop le\n");
    }

    return 0;
}