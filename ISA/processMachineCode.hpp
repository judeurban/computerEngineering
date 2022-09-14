#include <iostream>
#include "main.h"

void readMachineCode(void);
void processMachineCode(void);
bool zeroRegisterIsDestinationRegister(uint32_t*);
bool checkRegisterIndex(uint8_t*);

label* findLabelFromEnum(uint8_t);
uint8_t findIndexFromInstruction(uint32_t*);

static std::vector<label*> machineLabels;

void readMachineCode()
{
    // open the file
    std::ifstream instr_file(MACHINE_CODE_FILE);
    uint8_t bytes[4];
    uint32_t instruction;
    uint8_t byte;

    while (instr_file)
    {
        // load the entire instruction. All four bytes
        for(int i = 0 ; i < INSTRUCTION_SIZE ; i++)
        {
            byte = instr_file.get();

            // end of file!
            if(byte == (uint8_t)-1)
                return;

            bytes[i] = byte;
        }

        // copy the four bytes into ONE instruction
        memcpy(&instruction, &bytes[0], sizeof(bytes));
        instructions.push_back(instruction);

        // TODO: record any labels: here. Slide those into a buffer

        if(bytes[0] == L_V)
        {
            label* l = new label;
            l->enumerator = (uint8_t)bytes[1];
            l->instruction = &instruction;
            machineLabels.push_back(l);
        }
    }

    // decode each instruction (32 bits, 4 bytes)
    // EXCEPT when a float is encoded

    // have a FIFO buffer of six bytes or something. Some instructions may or may utilize all six bytes.

    // pseudocode:

    // opcode = bytes[0]
    // if opcode is an R-type:
        // for i in range(3):
            // find the three registers
        
        // do the operation

    // elif opcode is a J-Type:
        // ?

    // elif opcode is a I-Type:
        // register = bytes[1]

        // next FOUR bytes is the encoded float value
        // do the operation}

/**
 * @brief Checks to see if the passed register is the zero register, which you cannot write into.
*/

}

void processMachineCode()
{
    uint8_t opcode;
    uint16_t instruction_idx;
    
    uint8_t destination_register_idx;
    uint32_t* destination_register;

    uint8_t source_register1_idx;
    uint32_t* source_register1;

    uint8_t source_register2_idx;
    uint32_t* source_register2;

    uint32_t instruction;
    // uint32_t* currentInstruction;
    bool floatFLAG = false;
    uint8_t bytes[4];

    uint8_t jump_to_label_enum;

    // start on the FIRST instruction, initialize the programCounter
    instruction_idx = 0;
    programCounter = &instructions.at(instruction_idx);

    while(programCounter != NULL)
    {
        // update the program counter!
        programCounter = &instructions.at(instruction_idx);
        instruction = *programCounter;
        instruction_idx += 1;

        // break the while loop on the next iteration, assuming that this is at the end
        if(instruction_idx >= instructions.size())
        {
            programCounter = NULL;
        }
        
        // Read a float instead of an instruction
        if (floatFLAG)
        {
            floatFLAG = false;

            // process PREVIOUS instruction!
            if (opcode == LOADF_V)
            {
                memcpy(destination_register, &instruction, sizeof(bytes));
            }

            // operation complete, analyze the next instruciton
            continue;
        }

        // break apart the instruction into four bytes
        memcpy(&bytes, &instruction, sizeof(instruction));

        opcode = bytes[0];

        // R-Type
        if (opcode < JUMP_V)
        {
            // ignore if the register index is outta bounds
            if(checkRegisterIndex(&bytes[0]))
            {
                return;
            }

            // retrieve the register associate with that particular index
            destination_register = &allRegisters[bytes[1]];
            source_register1 = &allRegisters[bytes[2]];
            source_register2 = &allRegisters[bytes[3]];

            // ignore if they're tryna write into the zero register
            if(zeroRegisterIsDestinationRegister(destination_register)) 
                return;

            // TODO: make remainder of function calls here
            switch (opcode)
            {
            case ADDF_V:
                ADDF(destination_register, source_register1, source_register2);
                break;

            case DIVF_V:
                DIVF(destination_register, source_register1, source_register2);
                break;
            
            default:
                break;
            }
        }

        // J-Type
        else if(opcode < LOADI_V)
        {
            cout << "\n\nJ TYPE!!" << endl;
            jump_to_label_enum = bytes[1];

            // reset the PC incremetor to the index associaed with a paritcular instr
            programCounter = machineLabels.at(jump_to_label_enum)->instruction;
            instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);

            cout << "jumping to" << (char)(jump_to_label_enum+30) << endl;
            cout << "reset PC to" << (char)(jump_to_label_enum+30) << endl;
        }

        // I-Type
        else if(opcode < L_V)
        {
            // retrieve the register associate with that particular index
            destination_register = &allRegisters[bytes[1]];
            if(zeroRegisterIsDestinationRegister(destination_register))
                return;

            if(opcode == LOADF_V || opcode == LOADI_V)
            {
                // flag true so that the next four bytes read is the FLOAT value
                floatFLAG = true;
            }
            else if(opcode == CONSOLE_V)
            {
                CONSOLE(destination_register);
            }
        }
    }
}

bool zeroRegisterIsDestinationRegister(uint32_t* r1)
{
    if (r1 == zero_register)
    {
        cout << "cannot write into the zero register" << endl;
        return true;
    }

    return false;
}

bool checkRegisterIndex(uint8_t* bytes)
{
    if(bytes[1] > NUMBER_OF_REGISTERS-1 || bytes[2] > NUMBER_OF_REGISTERS-1 || bytes[3] > NUMBER_OF_REGISTERS-1)
    {
        cout << "invalid index for one or more of the registers" << endl;
        return true;
    }

    return false;
}

// label* findLabelFromEnum(uint8_t label_enum)
// {
//     label l;
//     for (std::vector<label*>::iterator it = machineLabels.begin() ; it != machineLabels.end(); ++it)
//     {
//         l = **it;
//         if (l.enumerator == label_enum)
//         {
//             // return it;
//             return &machineLabels.at(l.enumerator);
//         }
//     }

//     return NULL;
// }


uint8_t findIndexFromInstruction(uint32_t* instr)
{
    uint32_t instruction;
    uint8_t iter = 0;

    for (std::vector<uint32_t>::iterator it = instructions.begin() ; it != instructions.end(); ++it)
    {
        instruction = *it;
        if(instruction == *instr)
        {
            return iter;
        }

        iter += 1;
    }

    return 0;
}