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
#define ADDI_S "addi"
#define ADDI_V 0x0
#define ADDF_S "addf"
#define ADDF_V 0x1
#define SUBI_S "subi"
#define SUBI_V 0x2
#define MUL_S "mul"
#define MUL_V 0x3
#define DIV_S "div"
#define DIV_V 0x4
#define NOT_S "not"
#define NOT_V 0x5
#define AND_S "and"
#define AND_V 0x6
#define NAND_S "nand"
#define NAND_V 0x7
#define OR_S "or"
#define OR_V 0x8
#define NOR_S "nor"
#define NOR_V 0x9
#define XOR_S "xor"
#define XOR_V 0xa
#define SLT_S "slt"
#define SLT_V 0xb
#define JUMP_S "jump"
#define JUMP_V 0xc
#define BEQ_S "beq"
#define BEQ_V 0xd
#define LOADF_S "loadf"
#define LOADF_V 0xe
#define STORE_S "store"
#define STORE_V 0xf
#define CONSOLE_S "console"
#define CONSOLE_V 0x10

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

void processMachineCode(void);

#endif
