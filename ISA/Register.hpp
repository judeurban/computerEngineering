#include <iostream>

#define BYTE_SIZE 8             // bits
#define BYTES_PER_REGISTER 8         // bytes

class Register
{
    private:
        /* data */

    public:
        void* data_ptr;

    Register(/* args */)
    {
        data_ptr = (void*)calloc(BYTES_PER_REGISTER, BYTE_SIZE);

        if(!data_ptr)
        {
            std::cout << "Could not allocate memory for the register" << std::endl;
        }
        else
        {
            // std::cout << "Allocated " << BYTES_PER_REGISTER*BYTE_SIZE << " bits of memory for the register" << std::endl;
        }
    }

    ~Register()
    {
        // de-allocate memory from the register
        if(data_ptr)
        {
            free(data_ptr);
            // std::cout << "Deallocated " << BYTES_PER_REGISTER*BYTE_SIZE << " bits of memory for the register" << std::endl;
        }
    }

};
