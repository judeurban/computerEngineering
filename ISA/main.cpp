#include <iostream>
#include "main.h"
#include "generateMachineCode.hpp"
#include "processMachineCode.hpp"

bool compile = true;

int main(int argc, const char *argv[])
{
    if(compile)
    {
        readInstructions();
        generateMachineCode();
    }

    readMachineCode();
    processMachineCode();

    return 0;
}
