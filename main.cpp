#include <iostream>
#include <stdio.h>
#include "Parser.h"

int main() {
    CParser a;
    a.CheckGrammar("id + id * id");

    return 0;
}