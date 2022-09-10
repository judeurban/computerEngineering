#ifndef _INCL_GUARD
#define _INCL_GUARD

#include <bitset> // used for printing binary number representations using ascii valies
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#define NUMBER_OF_REGISTERS 8       //
#define MAXIMUM_INSTRUCTIONS 10     //
#define OPCODE_SIZE 4               // bits
#define REGISTER_INSTRUCTION_SIZE 4               // bits

// REGISTERS
uint32_t allRegisters[NUMBER_OF_REGISTERS];
uint32_t* zero_register = &allRegisters[0];

// OP-CODES

// maths

#define ADD_S "add"
#define ADD_V 0x0

#define SUB_S "sub"
#define SUB_V 0x1

#define MUL_S "mul"
#define MUL_V 0x2

#define DIV_S "div"
#define DIV_V 0x3

// logical

#define NOT_S "not"
#define NOT_V 0x4

#define AND_S "and"
#define AND_V 0x5

#define NAND_S "nand"
#define NAND_V 0x6

#define OR_S "or"
#define OR_V 0x7

#define NOR_S "nor"
#define NOR_V 0x8

#define XOR_S "xor"
#define XOR_V 0x9

// operations
#define SET_LESS_THAN_S "slt"
#define SET_LESS_THAN_V 0xA

// choose
#define JUMP_S "jump"
#define JUMP_V 0xB

#define BRANCH_S "beq"
#define BRANCH_V 0xC

// memory operations
#define LOAD_S "load"
#define LOAD_V 0xD

#define STORE_S "store"
#define STORE_V 0xE

#define CONSOLE_S "console"
#define CONSOLE_V 0xF

// functions
void readInstructions(void);
void printInstructions(void);
bool isValidCharacter(char);
void createRegisters(void);
void destroyRegisters(void);
void printRegisters(void);

// instruction business
std::vector<std::string> allInstructions;
const char invalidCharacters[] = {'\n', ' '};
const uint8_t invalidCharacterLen = sizeof(invalidCharacters)/sizeof(invalidCharacters[0]);

#define OPEN_INSTRUCTION_DELIMITER '('
#define CLOSE_INSTRUCTION_DELIMITER ')'
#define REGISTER_IDENTIFIER 'r'
#define REGISTER_DELIMTER ','

void generateMachineCode(void);
uint8_t generateOpcode(std::string);

#endif
