#include <iostream>
#include "main.h"

void readMachineCode(void);
void processMachineCode(void);
bool zeroRegisterIsDestinationRegister(uint32_t*);
bool checkRegisterIndex(uint8_t*);

void processMachineCode()
{
    // open the file
    std::ifstream instr_file(MACHINE_CODE_FILE);
    uint8_t opcode;

    uint8_t destination_register_idx;
    uint32_t* destination_register;

    uint8_t source_register1_idx;
    uint32_t* source_register1;

    uint8_t source_register2_idx;
    uint32_t* source_register2;

    uint8_t bytes[4];
    uint32_t instruction;
    bool floatFLAG = false;

    while (instr_file)
    {

        // load the entire instruction. All four bytes
        for(int i = 0 ; i < INSTRUCTION_SIZE ; i++)
        {
            bytes[i] = instr_file.get();
        }

        // check to see if the FLOAT flag is true. If it is, then process
        // the previous instruction that contained a float operation.
        if (floatFLAG)
        {
            floatFLAG = false;

            // process PREVIOUS instruction!
            if (opcode == LOADF_V)
            {
                memcpy(destination_register, bytes, sizeof(bytes));
            }

            // operation complete, analyze the next instruciton
            continue;
        }

        opcode = bytes[0];

        // R-Type
        if (opcode < JUMP_V)
        {

            if(checkRegisterIndex(&bytes[0]))
            {
                return;
            }

            // retrieve the register associate with that particular index
            destination_register = &allRegisters[bytes[1]];
            source_register1 = &allRegisters[bytes[2]];
            source_register2 = &allRegisters[bytes[3]];

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

        }

        // I-Type
        else
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
        // do the operation
}

void readMachineCode()
{
    return;
}

/**
 * @brief Checks to see if the passed register is the zero register, which you cannot write into.
*/
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