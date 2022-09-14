#include <iostream>
#include "main.h"

// functions
void readMachineCode(void);
void processMachineCode(void);
bool zeroRegisterIsDestinationRegister(uint32_t*);
bool checkRegisterIndex(uint8_t*);
uint8_t findIndexFromInstruction(uint32_t*);

// instructions
static std::vector<label*> machineLabels;               // contains enumerated labels with a pointer to the instruction
static std::vector<uint32_t*> instructions;             // contains pointers to instructions addresses in memory 
uint32_t* programCounter;                               // the address of the next instruction!

void readMachineCode()
{
    // open the file
    std::ifstream instr_file(MACHINE_CODE_FILE);
    uint8_t bytes[4];
    uint32_t* instruction_ptr;
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

        instruction_ptr = new uint32_t;

        // copy the four bytes into ONE instruction
        memcpy(instruction_ptr, &bytes[0], sizeof(bytes));
        instructions.push_back(instruction_ptr);

        // TODO: record any labels: here. Slide those into a buffer

        if(bytes[0] == L_V)
        {
            label* l = new label;
            l->enumerator = (uint8_t)bytes[1];
            l->instruction = instruction_ptr;
            machineLabels.push_back(l);
        }
    }
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
    bool immediateFLAG = false;
    uint8_t bytes[4];

    uint8_t jump_to_label_enum;

    // start on the FIRST instruction, initialize the programCounter
    instruction_idx = 0;
    programCounter = instructions.at(instruction_idx);

    while(programCounter != NULL)
    {
        // update the program counter!
        programCounter = instructions.at(instruction_idx);
        instruction = *programCounter;
        instruction_idx += 1;

        // break the while loop on the next iteration, assuming that this is at the end
        if(instruction_idx >= instructions.size())
        {
            programCounter = NULL;
        }
        
        // Read an immediate instead of an instruction
        if (immediateFLAG)
        {
            immediateFLAG = false;

            // process PREVIOUS instruction!
            switch (opcode)
            {
            case LOADF_V:
                LOADF(destination_register, &instruction);
                break;
            case LOADI_V:
                LOADI(destination_register, &instruction);
                break;
            default:
                break;
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
            if(zeroRegisterIsDestinationRegister(destination_register) && opcode != CONSOLE_V) 
                continue;

            // TODO: make remainder of function calls here
            switch (opcode)
            {
            case ADDF_V:
                ADDF(destination_register, source_register1, source_register2);
                break;

            case ADDI_V:
                ADDI(destination_register, source_register1, source_register2);
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
            switch (opcode)
            {
            case JUMP_V:
                jump_to_label_enum = bytes[1];
                // reset the PC incremetor to the index associaed with a paritcular instr
                // programCounter = machineLabels.at(jump_to_label_enum)->instruction;
                instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);
                break;
            case  BNE_V:

                source_register1 = &allRegisters[bytes[1]];
                source_register2 = &allRegisters[bytes[2]];

                if(BNE(source_register1, source_register2))
                {
                    jump_to_label_enum = bytes[3];

                    // reset the PC incremetor to the index associaed with a paritcular instr
                    instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);
                }
                
                break;          
            default:
                break;
            }

        }

        // I-Type
        else if(opcode < L_V)
        {
            // retrieve the register associate with that particular index
            destination_register = &allRegisters[bytes[1]];

            if(opcode == CONSOLE_V)
            {
                CONSOLE(destination_register);
                continue;
            }

            if(zeroRegisterIsDestinationRegister(destination_register))
                return;

            if(opcode == LOADF_V || opcode == LOADI_V)
            {
                // flag true so that the next four bytes read is the FLOAT value
                immediateFLAG = true;
            }
        }
    }
}

bool zeroRegisterIsDestinationRegister(uint32_t* r1)
{
    if (r1 == zero_register)
    {
        std::cout << "cannot write into the zero register" << std::endl;
        return true;
    }

    return false;
}

/* make sure register isn't out of bounds! */
bool checkRegisterIndex(uint8_t* bytes)
{
    if(bytes[1] > NUMBER_OF_REGISTERS-1 || bytes[2] > NUMBER_OF_REGISTERS-1 || bytes[3] > NUMBER_OF_REGISTERS-1)
    {
        std::cout << "invalid index for one or more of the registers" << std::endl;
        return true;
    }

    return false;
}

uint8_t findIndexFromInstruction(uint32_t* instr)
{
    uint8_t iter = 0;
    uint32_t* instruction_ptr;

    for (std::vector<uint32_t*>::iterator it = instructions.begin() ; it != instructions.end(); ++it)
    {
        if(*it == instr)
        {
            return iter;
        }

        iter += 1;
    }

    return 0;
}