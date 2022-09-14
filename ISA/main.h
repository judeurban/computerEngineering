#ifndef _INCL_GUARD
#define _INCL_GUARD

#include <bitset> // used for printing binary number representations using ascii valies
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include "operations.hpp"

#define NUMBER_OF_REGISTERS 8
#define INSTRUCTION_SIZE 4             // bytes

// #define ISA_ASSEMBLY_FILE "pi.asm"
// #define ISA_ASSEMBLY_FILE "for_loop.asm"
#define MACHINE_CODE_FILE "machine_code.bin"
std::string ISA_ASSEMBLY_FILE;

// REGISTERS
uint32_t allRegisters[NUMBER_OF_REGISTERS];
uint32_t* zero_register = &allRegisters[0];

const char stripCharacters[] = {'\n', ' ', '\t'};
const uint8_t stripCharactersLen = sizeof(stripCharacters)/sizeof(char);

// LABELS
struct label
{
    std::string text;
    uint8_t enumerator;
    uint32_t* instruction;
};

// =========================================
// OPCODES


// == R-TYPE ==
#define ADDI_S "addi"
#define ADDI_V 0x0
#define ADDF_S "addf"
#define ADDF_V 0x1
#define SUBI_S "subi"
#define SUBI_V 0x2
#define SUBF_S "subf"
#define SUBF_V 0x3
#define MULI_S "muli"
#define MULI_V 0x4
#define MULF_S "mulf"
#define MULF_V 0x5
#define DIVI_S "divi"
#define DIVI_V 0x6
#define DIVF_S "divf"
#define DIVF_V 0x7
#define SLT_S "slt"
#define SLT_V 0x8
#define NOT_S "not"
#define NOT_V 0x9
#define AND_S "and"
#define AND_V 0xa
#define NAND_S "nand"
#define NAND_V 0xb
#define OR_S "or"
#define OR_V 0xc
#define NOR_S "nor"
#define NOR_V 0xd
#define XOR_S "xor"
#define XOR_V 0xe

// == J-TYPE ==
#define JUMP_S "jump"
#define JUMP_V 0xf
#define BEQ_S "beq"
#define BEQ_V 0x10
#define BNE_S "bne"
#define BNE_V 0x11

// == I-TYPE ==
#define LOADI_S "loadi"
#define LOADI_V 0x12
#define LOADF_S "loadf"
#define LOADF_V 0x13
#define STORE_S "store"
#define STORE_V 0x14
#define CONSOLE_S "console"
#define CONSOLE_V 0x15

// == L-TYPE ==
#define L_S ":"
#define L_V 0x16

// total operations: 0x17 = Od23
// =========================================

#endif
