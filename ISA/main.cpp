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
        else if(opcode_byte < LOAD_V)
        {
            // TODO
        }
        // IMMEDIATE-TYPE
        else
        {
            if(opcode_byte == LOAD_V)
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
    if(opcode == ADD_S){
        return ADD_V;
    }
    else if(opcode == SUB_S){
        return SUB_V;
    }
    else if(opcode == MUL_S){
        return MUL_V;
    }
    else if(opcode == DIV_S){
        return DIV_V;
    }
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
    else if(opcode == SET_LESS_THAN_S){
        return SET_LESS_THAN_V;
    }
    else if(opcode == JUMP_S){
        return JUMP_V;
    }
    else if(opcode == BRANCH_S){
        return BRANCH_V;
    }
    else if(opcode == LOAD_S){
        return LOAD_V;
    }
    else if(opcode == STORE_S){
        return STORE_V;
    }
    else if(opcode == CONSOLE_S){
        return CONSOLE_V;
    }

    return 0;
}

void processMachineCode()
{
    // open the file
    std::ifstream instr_file(ISA_ASSEMBLY_FILE);

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

int main(int argc, const char *argv[])
{
    readInstructions();
    generateMachineCode();

    processMachineCode();

    return 0;
}
