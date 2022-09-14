#include <iostream>
#include "main.h"
#include "generateMachineCode.hpp"
#include "processMachineCode.hpp"

// bool compile = true;

int main(int argc, const char *argv[])
{
    if(argc > 1)
    {
        // get the filename from the console
        std::string file_arg(argv[1]);
        ISA_ASSEMBLY_FILE = file_arg;
        
        // read and generate instructions
        readInstructions();
        generateMachineCode();
    }

    readMachineCode();
    processMachineCode();

    return 0;
}