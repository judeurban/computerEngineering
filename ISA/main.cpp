#include <iostream>
#include "main.h"
#include "generateMachineCode.hpp"
#include "processMachineCode.hpp"

using namespace std;

bool compile = true;

// TODO: do not allow a write of the zero register
// TODO: J-type, enumerate the encoding of each label? Save for later.
// Reading the line, if you encounter a ':' character (or some other identifier), enumerate some index
// and store that instruction in memory (instruciton index) to be executed later

// int enumerater_i = 0
// vector labelVector;
// while(readingFile)
    // if newinstruction has a ':'
        // allStringInstructionsVector.push_back(newInstruction)
        // labelVector.push_back(&allStringInstructionsVector[end])

        // probably 

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
