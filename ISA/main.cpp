#include <iostream>
#include "main.h"

using namespace std;

// TODO: do not allow a write of the zero register

#define ISA_ASSEMBLY_FILE "main.asm"
#define MACHINE_CODE_FILE "machine_code.bin"

// TODO: J-type, enumerate the encoding of each label? Save for later.
// Reading the line, if you encounter a ':' character (or some other identifier), enumerate some index
// and store that instruction in memory (instruciton index) to be executed later

// int enumerater_i = 0
// vector labelVector;
// while(readingFile)
    // if newinstruction has a ':'
        // allInstructionsVector.push_back(newInstruction)
        // labelVector.push_back(&allInstructionsVector[end])

        // probably 

std::string stripInstruction(std::string &instruction)
{
    std::string stripped_instruction;
    char character;

    for (int i = 0; i < instruction.length(); i++)
    {
        character = instruction[i];
        if(isValidCharacter(character))
        {
            stripped_instruction.push_back(instruction[i]);
        }
    }

    return stripped_instruction;
}

bool isValidCharacter(char character)
{
    for(int i = 0 ; i < invalidCharacterLen; i++)
    {
        if (character == invalidCharacters[i])
        {
            return false;
        }
    }

    return true;
}

void readInstructions()
{
    std::ifstream instr_file(ISA_ASSEMBLY_FILE);
    std::string instr_string;
    char character;
    
    if (!instr_file.is_open()) {return;}

    while (instr_file)
    {
        character = instr_file.get();
        if (character < 0) break;

        // comments!
        if (character == '#')
        {
            // stall until the end of the line
            while((character = instr_file.get()) != '\n' && character != '\r' && character > 0) {}
        }

        // do not add any invalid characters (after the while loop above)
        if(character < 0) {continue;}
        
        // new line, add this instruction to the list of allInstructions
        if (instr_string.length() && character == '\n')
        {
            allInstructions.push_back(stripInstruction(instr_string));
            instr_string.erase();
        }
        // continue building the instruction string
        else if(character != '\n')
        {
            instr_string.push_back(character);
        }
    }

    // load the last instruction (the while loop terminates before we can add it)
    if (instr_string.length())
    {
        allInstructions.push_back(stripInstruction(instr_string));
        instr_string.erase();
    }
}

/**
 * @brief Prints the value in all of the registers
 * 
 */
void printRegisters()
{
    uint32_t registerData;
    for (int i = 0 ; i < NUMBER_OF_REGISTERS ; i++)
    {
        cout << "registerData for register " << i << " is ";
        registerData = allRegisters[i];
        cout << registerData << endl;
    }
}

void printInstructions()
{
    std::string instr_string;

    int i = 0;
    for (std::vector<std::string>::iterator it = allInstructions.begin() ; it != allInstructions.end(); ++it)
    {
        instr_string = *it;
        cout << "instruction " << i << " is >" << instr_string << "<" << endl;
        i++;
    }
}

void generateMachineCode()
{
    std::ofstream machine_code_file(MACHINE_CODE_FILE, std::ios::out | std::ios::binary);
    size_t delimter_position;
    size_t register_delimter_position;

    std::string instruction_string;
    std::string sub_str;
    // std::string register_str;

    uint8_t opcode_byte;
    uint8_t register_byte;
    float immediate_float;

    if(!machine_code_file.is_open()) { return; }

    // TODO: left shift by 4, then AND with the register to create represent each with four Fbits?
    // NOTE: using BIG endian encoding

    for (std::vector<std::string>::iterator it = allInstructions.begin() ; it != allInstructions.end(); ++it)
    {
        // find the instruction string
        instruction_string = *it;

        // get the delimeter position of the first open parenthesis
        delimter_position = instruction_string.find(OPEN_INSTRUCTION_DELIMITER);
        if(delimter_position == std::string::npos) { return; }

        // convert the opcode ascii representation into a single byte
        sub_str = instruction_string.substr(0, delimter_position);
        opcode_byte = generateOpcode(sub_str);

        // write the opcode to the file
        machine_code_file.write(reinterpret_cast<const char*>(&opcode_byte), sizeof(uint8_t));

        // increment the string forward, 1 past the parenthesis. This is the register command.
        instruction_string.erase(0, delimter_position + sizeof(OPEN_INSTRUCTION_DELIMITER));

        // find the command, (between the parenthesis)
        delimter_position = instruction_string.find(CLOSE_INSTRUCTION_DELIMITER);
        instruction_string = instruction_string.substr(0, delimter_position);

        // REGISTER-TYPE
        if(opcode_byte < JUMP_V)
        {
            // look for REGISTER_IDENTIFIER ('r's)
            while((delimter_position = instruction_string.find(REGISTER_IDENTIFIER)) != std::string::npos)
            {
                register_delimter_position = instruction_string.find(REGISTER_DELIMTER);
                sub_str = instruction_string.substr(delimter_position + 1, register_delimter_position - 1);
                instruction_string.erase(0,  sizeof(REGISTER_IDENTIFIER) + sub_str.length() + sizeof(REGISTER_DELIMTER));
                
                // write the register location as machine code
                register_byte = (uint8_t)std::stoi(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
            }
        }
        // JUMP-TYPE
        else if(opcode_byte < LOADI_V)
        {
            // TODO
        }
        // IMMEDIATE-TYPE
        else
        {
            if(opcode_byte == LOADF_V)
            {

                // Find the register number
                delimter_position = instruction_string.find(REGISTER_IDENTIFIER);
                register_delimter_position = instruction_string.find(REGISTER_DELIMTER);
                sub_str = instruction_string.substr(delimter_position + 1, register_delimter_position - 1);
                instruction_string.erase(0,  sizeof(REGISTER_IDENTIFIER) + sub_str.length() + sizeof(REGISTER_DELIMTER));
                
                // write the register location as machine code
                register_byte = (uint8_t)std::stoi(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

                // find the ')'
                delimter_position = instruction_string.find(CLOSE_INSTRUCTION_DELIMITER);
                sub_str = instruction_string.substr(0, delimter_position - 1);

                // pad the rest of the instruction with zeros
                register_byte = 0;
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

                // write the float value as machine code
                immediate_float = std::stof(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&immediate_float), sizeof(float));

            }
            else if(opcode_byte = CONSOLE_V)
            {
                // Find the register number
                delimter_position = instruction_string.find(REGISTER_IDENTIFIER);
                register_delimter_position = instruction_string.find(REGISTER_DELIMTER);
                sub_str = instruction_string.substr(delimter_position + 1, register_delimter_position - 1);
                instruction_string.erase(0,  sizeof(REGISTER_IDENTIFIER) + sub_str.length() + sizeof(REGISTER_DELIMTER));

                // write the register location as machine code
                register_byte = (uint8_t)std::stoi(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

                // pad the rest of the instruction with zeros
                register_byte = 0;
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

            }
           
        }
    }

    // TODO branch between different types of opcodes. (r-type, j-type, i-type) accordingly.
}

uint8_t generateOpcode(std::string opcode)
{
    // MATH, INTEGERS
    if(opcode == ADDI_S){
        return ADDI_V;
    }
    else if(opcode == SUBI_S){
        return SUBI_V;
    }
    else if(opcode == MULI_S){
        return MULI_V;
    }
    else if(opcode == DIVI_S){
        return DIVI_V;
    }

    // MATH, FLOATS
    else if(opcode == ADDF_S){
        return ADDF_V;
    }
    else if(opcode == SUBF_S){
        return SUBF_V;
    }
    else if(opcode == MULF_S){
        return MULF_V;
    }
    else if(opcode == DIVF_S){
        return DIVF_V;
    }

    // LOGICAL
    else if(opcode == NOT_S){
        return NOT_V;
    }
    else if(opcode == AND_S){
        return AND_V;
    }
    else if(opcode == NAND_S){
        return NAND_V;
    }
    else if(opcode == OR_S){
        return OR_V;
    }
    else if(opcode == NOR_S){
        return NOR_V;
    }
    else if(opcode == XOR_S){
        return XOR_V;
    }
    else if(opcode == SLT_S){
        return SLT_V;
    }
    else if(opcode == JUMP_S){
        return JUMP_V;
    }
    else if(opcode == BEQ_S){
        return BEQ_V;
    }
    else if(opcode == STORE_S){
        return STORE_V;
    }
    else if(opcode == CONSOLE_S){
        return CONSOLE_V;
    }

    // MEMORY, INTEGERS
    else if(opcode == LOADI_S){
        return LOADI_V;
    }
    // MEMORY, FLOATS
    else if(opcode == LOADF_S){
        return LOADF_V;
    }

    return -1;
}

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
        for(int i = 0 ; i < 4 ; i++)
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
        }

        opcode = bytes[0];

        // R-Type
        if (opcode < JUMP_V)
        {
            // destination_register_idx = bytes[1];
            // source_register1_idx = bytes[2];
            // source_register2_idx = bytes[3];

            destination_register = &allRegisters[bytes[1]];
            source_register1 = &allRegisters[bytes[2]];
            source_register2 = &allRegisters[bytes[3]];

            // TODO: make remainder of function calls here
            if(opcode == DIVF_V)
            {
                DIVF(destination_register, source_register1, source_register2);
            }
        }

        // J-Type
        else if(opcode < LOADI_V)
        {

        }

        // I-Type
        else
        {
            destination_register_idx = bytes[1];
            destination_register = &allRegisters[destination_register_idx];

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

void DIVF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    float rd_value;
    float r1_value;
    float r2_value;

    memcpy(&r1_value, r1, sizeof(float));
    memcpy(&r2_value, r2, sizeof(float));

    // operation
    if (r2_value == 0.0)
    {
        cout << "you cannot divide by zero" << endl;
        rd_value = -1.0;
    }
    else
    {
        rd_value =  r1_value / r2_value;
    }

    // memory write
    memcpy(r_destination, &rd_value, sizeof(float));
}

void CONSOLE(uint32_t* registerToPrint)
{
    printf("0x%x", (uint32_t) *registerToPrint);
}

int main(int argc, const char *argv[])
{
    readInstructions();
    generateMachineCode();

    processMachineCode();

    return 0;
}
