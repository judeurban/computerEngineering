#include <iostream>
#include "main.h"

// functions
void buildFunctionPointerArray(void);
int readMachineCode(void);
int processMachineCode(void);
void processRType(uint8_t, uint32_t*, uint32_t*, uint32_t*);
bool zeroRegisterIsDestinationRegister(uint32_t*);
bool checkRegisterIndex(uint8_t*);
uint8_t findIndexFromInstruction(uint32_t*);

// instructions
static std::vector<label*> machineLabels;               // contains enumerated labels with a pointer to the instruction
static std::vector<uint32_t*> instructions;             // contains pointers to instructions addresses in memory 
uint32_t* programCounter;                               // the address of the next instruction!

void buildFunctionPointerArray()
{
  RTypeFunctionPtrs[ADDI_V] = ADDI; /* address of ADDI() */
  RTypeFunctionPtrs[ADDF_V] = ADDF; /* address of ADDF() */
  RTypeFunctionPtrs[SUBI_V] = SUBI; /* address of SUBI() */
  RTypeFunctionPtrs[SUBF_V] = SUBF; /* address of SUBF() */
  RTypeFunctionPtrs[MULI_V] = MULI; /* address of MULI() */
  RTypeFunctionPtrs[MULF_V] = MULF; /* address of MULF() */
  RTypeFunctionPtrs[DIVI_V] = DIVI; /* address of DIVI() */
  RTypeFunctionPtrs[DIVF_V] = DIVF; /* address of DIVF() */
//   RTypeFunctionPtrs[NOT_V] = NOT; /* address of NOT() */
  RTypeFunctionPtrs[AND_V] = AND; /* address of AND() */
  RTypeFunctionPtrs[NAND_V] = NAND; /* address of NAND() */
  RTypeFunctionPtrs[OR_V] = OR; /* address of OR() */
  RTypeFunctionPtrs[NOR_V] = NOR; /* address of NOR() */
  RTypeFunctionPtrs[XOR_V] = XOR; /* address of XOR() */
  RTypeFunctionPtrs[XNOR_V] = XNOR; /* address of XOR() */
}

int readMachineCode()
{
    // open the file
    std::ifstream machine_code_file(MACHINE_CODE_FILE);
    uint8_t bytes[4];
    uint32_t* instruction_ptr;
    uint8_t byte;

    if (!machine_code_file.is_open())
    {
        std::cout << "\nERROR: There is no machine code file." << std::endl;
        std::cout << "You must first generate a machine code file by specificing it as an option when running main.exe" << std::endl;
        std::cout << "\nTry: ./main.exe pi.asm\n" << std::endl;
        return COMPILER_ERROR;
    }

    while (machine_code_file)
    {
        // load the entire instruction. All four bytes
        for(int i = 0 ; i < INSTRUCTION_SIZE ; i++)
        {
            byte = machine_code_file.get();

            // end of file!
            if(byte == (uint8_t)-1)
                return COMPILER_SUCCESS;

            bytes[i] = byte;
        }

        instruction_ptr = new uint32_t;

        // copy the four bytes into ONE instruction
        memcpy(instruction_ptr, &bytes[0], sizeof(bytes));
        instructions.push_back(instruction_ptr);

        if(bytes[0] == L_V)
        {
            label* l = new label;
            l->enumerator = (uint8_t)bytes[1];
            l->instruction = instruction_ptr;
            machineLabels.push_back(l);
        }
    }

    machine_code_file.close();
    return COMPILER_SUCCESS;
}

int processMachineCode()
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
    bool immediateFLAG = false;
    uint8_t bytes[4];

    uint8_t jump_to_label_enum;

    // do not proceess empty an empty vector of instructions
    if (instructions.size() == 0)
    {
        return COMPILER_SUCCESS;
    }

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
            if(opcode < NOT_V && checkRegisterIndex(&bytes[0]))
            {
                return COMPILER_ERROR;
            }

            // retrieve the register associate with that particular index
            destination_register = &allRegisters[bytes[1]];
            source_register1 = &allRegisters[bytes[2]];
            source_register2 = &allRegisters[bytes[3]];

            // ignore if they're tryna write into the zero register
            if(zeroRegisterIsDestinationRegister(destination_register) && opcode != CONSOLE_V) 
                continue;

            if (opcode == NOT_V)
            {
                NOT(destination_register);
            }

            else
            {
                processRType(opcode, destination_register, source_register1, source_register2);
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
                    instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);
                    break;

                case BNE_V:
                case BEQ_V:

                    source_register1 = &allRegisters[bytes[1]];
                    source_register2 = &allRegisters[bytes[2]];

                    if(BNE_V && BNE(source_register1, source_register2))
                    {
                        jump_to_label_enum = bytes[3];

                        // reset the PC incremetor to the index associaed with a paritcular instr
                        instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);
                        programCounter = instructions.at(instruction_idx);
                    }
                    if(BEQ_V && BEQ(source_register1, source_register2))
                    {
                        jump_to_label_enum = bytes[3];

                        // reset the PC incremetor to the index associaed with a paritcular instr
                        instruction_idx = findIndexFromInstruction(machineLabels.at(jump_to_label_enum)->instruction);
                        programCounter = instructions.at(instruction_idx);
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
                return COMPILER_ERROR;

            if(opcode == LOADF_V || opcode == LOADI_V)
            {
                // flag true so that the next four bytes read is the FLOAT value
                immediateFLAG = true;
            }
        }
    }

    return COMPILER_SUCCESS;
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

void processRType(uint8_t opcode, uint32_t* destination_register, uint32_t* source_register1, uint32_t* source_register2)
{
    // make the function call with the corresponding opcode!
    (*RTypeFunctionPtrs[opcode])(destination_register, source_register1, source_register2);
}