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

    std::string instr_string;
    size_t delim_position = 0;
    
    std::string opcode_string;
    std::bitset<OPCODE_SIZE> opcode;
    std::vector<std::bitset<REGISTER_INSTRUCTION_SIZE>> register_array;             // vector of register addresses, may be one or multiple.
    // std::bitset<REGISTER_INSTRUCTION_SIZE> register;
    char register_number_character;

    std::string machine_code_instruction;

    // TODO: write binary file. For now, just write machine code using ascii.

    // iterate through the list of instructions
    // output the machine-code instruction for each
    // start with just the op-code, since that's all we have.

    if(!machine_code_file.is_open()) { return; }

    // iterate through the list of instructions
    for (std::vector<std::string>::iterator it = allInstructions.begin() ; it != allInstructions.end(); ++it)
    {
        // find the instruction string
        instr_string = *it;

        // get the delimeter position of the first parenthesis
        delim_position = instr_string.find(OPEN_INSTRUCTION_DELIMITER);
        if(delim_position == std::string::npos) { return; }

        opcode_string = instr_string.substr(0, delim_position).c_str();
        opcode = generateOpcode(opcode_string);
        instr_string.erase(0, delim_position + sizeof(OPEN_INSTRUCTION_DELIMITER));

        // find 'r'
        while ( (delim_position = instr_string.find(REGISTER_IDENTIFIER)) != std::string::npos)
        {
            register_number_character = *instr_string.substr(delim_position + 1).c_str();
            instr_string.erase(0, delim_position + sizeof(OPEN_INSTRUCTION_DELIMITER));
            std::bitset<REGISTER_INSTRUCTION_SIZE> register_bitset((int)register_number_character);
            register_array.push_back(register_bitset);
        }

        // write the to the file
        machine_code_file << opcode;

        for (std::vector<std::bitset<REGISTER_INSTRUCTION_SIZE>>::iterator i = register_array.begin() ; i != register_array.end(); ++i)
        {
            std::bitset<REGISTER_INSTRUCTION_SIZE> register_index = *i;
            machine_code_file << register_index;
        }

        register_array.clear();

        machine_code_file << endl;
    }

    // TODO: branch between different types
}

std::bitset<OPCODE_SIZE> generateOpcode(std::string opcode)
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
