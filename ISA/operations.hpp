#include <iostream>
#include "main.h"

using namespace std;

// float representation
static float rd_f;
static float r1_f;
static float r2_f;

// signed int representation
static int rd_i;
static int r1_i;
static int r2_i;

void DIVF(uint32_t*, uint32_t*, uint32_t*);
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



void DIVF(uint32_t* r_destination, uint32_t* r1, uint32_t* r2)
{
    memcpy(&r1_f, r1, sizeof(float));
    memcpy(&r2_f, r2, sizeof(float));

    // operation
    if (r2_f == 0.0)
    {
        cout << "you cannot divide by zero" << endl;
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
    printf("0x%x", (uint32_t) *registerToPrint);
}
