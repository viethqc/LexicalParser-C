#include <iostream>
#include <stdio.h>
#include "Parser.h"

int main() {
    CParser a;
    if (a.CheckGrammar("id + id") == true)
    {
        printf("Van pham hop le\n");
    }
    else
    {
        printf("Van pham khong hop le\n");
    }

    return 0;
}