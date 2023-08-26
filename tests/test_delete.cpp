#include "StackMatrix.cpp"
#include <iostream>

int32_t main()
{
    StackMatrix<uint32_t, 16, 10> matrix;

    for(uint32_t j = 0; j < 10; j++)
        for(uint32_t i = 0; i < 16; i++)
            matrix.get(i, j) = j*16+i;

    for(uint32_t j = 0; j < 10; j++)
    {
        for(uint32_t i = 0; i < 16; i++)
            std::cout << matrix.get(i, j) << ' ';
        std::cout << '\n';
    }
    std::cout << std::flush;
}
