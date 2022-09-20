#include <iostream>
#include "main.h"

// float representation
static float rd_f;
static float r1_f;
static float r2_f;

// signed int representation
static uint32_t rd_i;
static uint32_t r1_i;
static uint32_t r2_i;

void LOADF(uint32_t*, uint32_t*);
void DIVF(uint32_t*, uint32_t*, uint32_t*);
void ADDI(uint32_t*, uint32_t*, uint32_t*);
int BNE(uint32_t*, uint32_t*);
void CONSOLE(uint32_t*);

//ADDF
void ADDF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));

    // operation
    rd_f =  r1_f + r2_f;

    // memory write
    memcpy(r_destination, &rd_f, sizeof(float));
}

//ADDI
void ADDI(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(int));
    memcpy(&r2_i, r2, sizeof(int));
    
    std::cout << "wow" << std::endl;

    // operation
    rd_i =  r1_i + r2_i;

    // memory write
    memcpy(r_destination, &rd_i, sizeof(int));
    
}

//LOADF
void LOADF(uint32_t* r_destination, uint32_t* instruction)
{
    memcpy(r_destination, instruction, sizeof(instruction));
    
}

void LOADI(uint32_t* r_destination, uint32_t* instruction)
{
    memcpy(r_destination, instruction, sizeof(uint32_t));
    
}

//BNE
int BNE(uint32_t* r1, uint32_t* r2)
{
    // TRUE if they're not equal
    if(*r1 != *r2)
    {
        return 1;
    }

    // False otherwise
    return 0;
}

//DIVF
void DIVF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));

    // operation
    if (r2_f == 0.0)
    {
        std::cout << "you cannot divide by zero" << std::endl;
        rd_f = -1.0;
    }
    else
    {
        rd_f =  r1_f / r2_f;
    }

    // memory write
    memcpy(r_destination, &rd_f, sizeof(float));
}


void DIVI(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(uint32_t));
    memcpy(&r2_f, r2, sizeof(uint32_t));

    // operation
    if (r2_f == 0)
    {
        std::cout << "you cannot divide by zero" << std::endl;
        rd_f = -1;
    }
    else
    {
        rd_f =  r1_f / r2_f;
    }

    // memory write
    memcpy(r_destination, &rd_f, sizeof(uint32_t));
}

//SUBBI
void SUBI(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(int));
    memcpy(&r2_i, r2, sizeof(int));

    //operation
    rd_i = r1_i - r2_i;

    // memory write
    memcpy(r_destination, &rd_i, sizeof(float));
    
}

//SUBBF
void SUBF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));
    
    //operation
    rd_f = r1_f - r2_f;

    // memory write
    memcpy(r_destination, &rd_f, sizeof(float));
    
}

//MULI
void MULI(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(int));
    memcpy(&r2_i, r2, sizeof(int));
    
    //operation
    rd_i = r1_i * r2_i;
    
    // memory write
    memcpy(r_destination, &rd_i, sizeof(float));
}

//MULF
void MULF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));
    
    //operation
    rd_f = r1_f * r2_f;
    
    // memory write
    memcpy(r_destination, &rd_f, sizeof(float));
    
}


//NAND
void NAND(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(uint32_t));
    memcpy(&r2_i, r2, sizeof(uint32_t));
    
    //operation
    rd_f = !(r1_i & r2_i);
    
    // memory write
    memcpy(r_destination, &rd_f, sizeof(uint32_t));
    
}

//OR
void OR(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(int));
    memcpy(&r2_i, r2, sizeof(int));
    
    //operation
    rd_i = r1_i | r2_i;
    
    // memory write
    memcpy(r_destination, &rd_i, sizeof(float));
    
}


//AND
void AND(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(uint32_t));
    memcpy(&r2_i, r2, sizeof(uint32_t));
    
    //operation
    rd_i = r1_i & r2_i;
    
    // memory write
    memcpy(r_destination, &rd_i, sizeof(uint32_t));
  
}

//NOR
void NOR(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
  memcpy(&r1_i, r1, sizeof(uint32_t));
  memcpy(&r2_i, r2, sizeof(uint32_t));
  
  //operation
  rd_f = !(r1_i|r2_i);
  
  // memory write
  memcpy(r_destination, &rd_i, sizeof(uint32_t));
  
}

//XOR
void XOR(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
  memcpy(&r1_f, r1, sizeof(uint32_t));
  memcpy(&r2_f, r2, sizeof(uint32_t));
  
  //operation
  // rd_f = !(r1_f ^ r2_f);
  
  // memory write
  memcpy(r_destination, &rd_f, sizeof(uint32_t));
  

} // Needs testing

//JUMP
//BEQ
//STORE
//SLT 
//NOT

//CONSOLE
void CONSOLE(uint32_t* registerToPrint)
{
    printf("Register 0x%x contains the value 0x%x\n", registerToPrint, (uint32_t) *registerToPrint);
    
}
