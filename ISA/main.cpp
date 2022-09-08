#include <iostream>
#include <cstring>
// #include "Register.hpp"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define NUMBER_OF_REGISTERS 8
#define MAXIMUM_INSTRUCTIONS 10

// registers
uint32_t allRegisters[NUMBER_OF_REGISTERS];
void createRegisters(void);
void destroyRegisters(void);
void configureZeroRegister(void);
void printRegisters(void);

// instructions
std::vector<std::string> allInstructions;
void readInstructions(void);
void printInstructions(void);
bool isValidCharacter(char);

const char invalidCharacters[] = {'\n', ' '};

// TODO: do not allow a write of zero register

std::string stripInstruction(std::string &instruction)
{
    std::string stripped_instruction;
    char character;

    for (int i = 0; i < instruction.length() ; i++)
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
    char invalid_character;
    for(int i = 0 ; i < sizeof(invalidCharacters)/sizeof(invalidCharacters[0]) ; i++)
    {
        invalid_character = invalidCharacters[i];
        if (character == invalid_character)
        {
            return false;
        }
    }

    return true;
}

void createRegisters()
{
    configureZeroRegister();

}

// void destroyRegisters()
// {
//     // Register* register_ptr;

//     for(int i = 0 ; i < NUMBER_OF_REGISTERS ; i++)
//     {
//         register_ptr = allRegisters[i];
//         register_ptr->~Register();
//     }
// }

void configureZeroRegister()
{
    // Register* zero_register = allRegisters[0];
    uint32_t* zero_register = &allRegisters[0];
    // cout << "zero_register's data is " << *(uint64_t*)zero_register->data_ptr << endl;
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

            cout << "instr_string.length()" << instr_string.length() << endl;

            // new line, add this instruction to the list of allInstructions
            if (instr_string.length() && character == '\n')
            {
                allInstructions.push_back(stripInstruction(instr_string));
                cout << "ADDING INSTRUCTION TO VECTOR" << instr_string << endl;
                instr_string.clear();
            }
            // continue building the instruction string
            else
            {
                instr_string.push_back(character);
            }

        }

        cout << "instr_string.length() is " << instr_string.length() << endl;

        // load the last instruction (the while loop terminates before we can add it)
        if (instr_string.length())
        {
            allInstructions.push_back(instr_string);
        }

        printInstructions();

    }

}

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

int main(int argc, const char *argv[])
{
    createRegisters();
    printRegisters();

    readInstructions();
    // printInstructions();


    // destroyRegisters();
    return 0;
}
