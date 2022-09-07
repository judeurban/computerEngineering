#include <iostream>

#define REGISTER_SIZE 8

class Register
{
    private:
        /* data */
        void* data_ptr;

    public:

    Register(/* args */)
    {
        data_ptr = (void*)malloc(REGISTER_SIZE);
        if(!data_ptr)
        {
            std::cout << "Could not allocate memory for the register" << std::endl;
        }
        else
        {
            std::cout << "Allocated " << REGISTER_SIZE*8 << " bits of memory for the register" << std::endl;
        }
    }

    ~Register()
    {
    }

};
