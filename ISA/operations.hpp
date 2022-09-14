#include <iostream>
#include "main.h"

// using namespace std;

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
int BNE(uint32_t*, uint32_t*);
void CONSOLE(uint32_t*);

void ADDF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));

    // operation
    rd_f =  r1_f + r2_f;

    // memory write
    memcpy(r_destination, &rd_f, sizeof(float));
}

void ADDI(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_i, r1, sizeof(int));
    memcpy(&r2_i, r2, sizeof(int));

    // operation
    rd_i =  r1_i + r2_i;

    // memory write
    memcpy(r_destination, &rd_i, sizeof(float));
}

void LOADF(uint32_t* r_destination, uint32_t* instruction)
{
    memcpy(r_destination, instruction, sizeof(instruction));
}

void LOADI(uint32_t* r_destination, uint32_t* instruction)
{
    memcpy(r_destination, instruction, sizeof(instruction));
}

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

void CONSOLE(uint32_t* registerToPrint)
{
    printf("Register 0x%x contains the value 0x%x\n", registerToPrint, (uint32_t) *registerToPrint);
}
