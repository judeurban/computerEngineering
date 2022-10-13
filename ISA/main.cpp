#include <iostream>
#include "main.h"
#include "generateMachineCode.hpp"
#include "processMachineCode.hpp"

int main(int argc, const char *argv[])
{
    buildFunctionPointerArray();

    if(argc > 1)
    {
        // get the filename from the console
        std::string file_arg(argv[1]);
        ISA_ASSEMBLY_FILE = file_arg;
        
        // read and generate instructions
        if(readInstructions() || generateMachineCode())
        {
            return COMPILER_ERROR;
        }
    }

    readMachineCode();
    processMachineCode();

    return COMPILER_SUCCESS;
}