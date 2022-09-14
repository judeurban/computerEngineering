#include <iostream>
#include "main.h"

using namespace std;

#define OPEN_INSTRUCTION_DELIMITER '('
#define CLOSE_INSTRUCTION_DELIMITER ')'
#define REGISTER_IDENTIFIER 'r'
#define REGISTER_DELIMTER ','

// functions
void readInstructions(void);
void printInstructions(void);
bool isValidCharacter(char);
void createRegisters(void);
void destroyRegisters(void);
void printRegisters(void);
void generateMachineCode(void);
uint8_t generateOpcode(std::string);

static std::vector<label*> allLabels;
uint8_t findEnumFromLabel(std::string);

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
    for(int i = 0 ; i < stripCharactersLen; i++)
    {
        if (character == stripCharacters[i])
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
        
        // new line, add this instruction to the list of allStringInstructions
        if (instr_string.length() && character == '\n')
        {
            instr_string = stripInstruction(instr_string);
            if(instr_string.length() > 0)
            {
                allStringInstructions.push_back(stripInstruction(instr_string));
                instr_string.erase();
            }
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
        instr_string = stripInstruction(instr_string);
        if(instr_string.length() > 0)
        {
            allStringInstructions.push_back(stripInstruction(instr_string));
            instr_string.erase();
        }
    }

    // printInstructions();
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
    for (std::vector<std::string>::iterator it = allStringInstructions.begin() ; it != allStringInstructions.end(); ++it)
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
    uint8_t label_enumerator = 0;
    float immediate_float;
    int immediate_int;

    if(!machine_code_file.is_open()) { return; }

    // TODO: left shift by 4, then AND with the register to create represent each with four Fbits?
    // NOTE: using BIG endian encoding

    for (std::vector<std::string>::iterator it = allStringInstructions.begin() ; it != allStringInstructions.end(); ++it)
    {
        // find the instruction string
        instruction_string = *it;

        // labels!
        if(instruction_string.find(L_S) != std::string::npos)
        {
            // 1 byte for the opcode
            // 1 byte for the enumerator
            // 2 bytes are wasted...

            sub_str = instruction_string.substr(0, instruction_string.size()-1);

            // we will utilize register_byte to prevent creating a semi-useless variable

            opcode_byte = L_V;
            register_byte = label_enumerator;
            label_enumerator += 1;

            label* l = new label;
            l->text = sub_str;
            l->enumerator = (uint8_t)allLabels.size();
            allLabels.push_back(l);

            machine_code_file.write(reinterpret_cast<const char*>(&opcode_byte), sizeof(uint8_t));
            machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

            // :(
            register_byte = 0;
            machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
            machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));

            continue;
        }

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
            switch (opcode_byte)
            {
            case JUMP_V:
                label_enumerator = findEnumFromLabel(instruction_string);

                // :( so much waste
                machine_code_file.write(reinterpret_cast<const char*>(&label_enumerator), sizeof(uint8_t));
                machine_code_file.write(reinterpret_cast<const char*>(&label_enumerator), sizeof(uint8_t));
                machine_code_file.write(reinterpret_cast<const char*>(&label_enumerator), sizeof(uint8_t));
                break;
            case BNE_V:

                // register1
                delimter_position = instruction_string.find(REGISTER_DELIMTER);
                sub_str = instruction_string.substr(instruction_string.find(REGISTER_IDENTIFIER)+1, delimter_position-1);
                register_byte = (uint8_t)std::stoi(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
                instruction_string = instruction_string.erase(0, delimter_position+1);

                // register2
                delimter_position = instruction_string.find(REGISTER_DELIMTER);
                sub_str = instruction_string.substr(instruction_string.find(REGISTER_IDENTIFIER)+1, delimter_position-1);
                register_byte = (uint8_t)std::stoi(sub_str.c_str());
                machine_code_file.write(reinterpret_cast<const char*>(&register_byte), sizeof(uint8_t));
                instruction_string = instruction_string.erase(0, delimter_position+1);

                // label enum
                label_enumerator = findEnumFromLabel(instruction_string);
                machine_code_file.write(reinterpret_cast<const char*>(&label_enumerator), sizeof(uint8_t));
                break;
            
            default:
                break;
            }


        }

        // IMMEDIATE-TYPE
        else
        {
            if(opcode_byte == LOADF_V || opcode_byte == LOADI_V)
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


                    switch (opcode_byte)
                    {
                    case LOADF_V:
                        // write the float value as machine code
                        immediate_float = std::stof(sub_str.c_str());
                        machine_code_file.write(reinterpret_cast<const char*>(&immediate_float), sizeof(float));
                        break;
                    
                    case LOADI_V:
                        // write the int value as machine code
                        immediate_int = std::stoi(sub_str.c_str());
                        machine_code_file.write(reinterpret_cast<const char*>(&immediate_int), sizeof(int));
                        break;
                    default:
                        break;
                    }
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
    else if(opcode == BNE_S){
        return BNE_V;
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

uint8_t findEnumFromLabel(std::string label_text)
{
    label l;
    for (std::vector<label*>::iterator it = allLabels.begin() ; it != allLabels.end(); ++it)
    {
        l = **it;
        if (l.text == label_text)
        {
            return l.enumerator;
        }
    }

    return -1;
}