#include <iostream>
#include "main.h"

using namespace std;

// TODO: do not allow a write of the zero register

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
    std::ifstream instr_file("main.asm");
    std::string instr_string;
    char character;

    if (instr_file.is_open()) 
    {
        while (instr_file)
        {
            character = instr_file.get();
            
            // saturate null terminator 
            if (character == -1)
            {
                character = 0;
            }

            // new line, add this instruction to the list of allInstructions
            if (instr_string.length() && character == '\n')
            {
                allInstructions.push_back(stripInstruction(instr_string));
                instr_string.clear();
            }
            // continue building the instruction string
            else
            {
                instr_string.push_back(character);
            }
        }

        // load the last instruction (the while loop terminates before we can add it)
        if (instr_string.length())
        {
            allInstructions.push_back(stripInstruction(instr_string));
        }

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
    std::ofstream machine_code_file("machine_code_file.txt");
    size_t delimter_position;
    size_t register_delimter_position;

    std::string instruction_string;
    std::string sub_str;
    // std::string register_str;

    uint8_t opcode_byte;
    uint8_t register_byte;

    if(!machine_code_file.is_open()) { return; }

    // TODO: left shift by 4, then AND with the register to create represent each with four bits?

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

        machine_code_file << opcode_byte;

        // increment the string forward, 1 past the parenthesis. This is the register command.
        instruction_string.erase(0, delimter_position + sizeof(OPEN_INSTRUCTION_DELIMITER));

        // find the command, (between the parenthesis)
        delimter_position = instruction_string.find(CLOSE_INSTRUCTION_DELIMITER);
        instruction_string = instruction_string.substr(0, delimter_position);

        // look for REGISTER_IDENTIFIER ('r's)
        while((delimter_position = instruction_string.find(REGISTER_IDENTIFIER)) != std::string::npos)
        {
            register_delimter_position = instruction_string.find(REGISTER_DELIMTER);
            sub_str = instruction_string.substr(delimter_position + 1, register_delimter_position - 1);
            cout << "operation includes register " << sub_str << endl;
            instruction_string.erase(0,  sizeof(REGISTER_IDENTIFIER) + sub_str.length() + sizeof(REGISTER_DELIMTER));
            
            machine_code_file << (uint8_t)std::stoi(sub_str.c_str());
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
    else if(opcode == EGG_S){
        return EGG_V;
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    readInstructions();
    // printInstructions();
    generateMachineCode();

    return 0;
}
