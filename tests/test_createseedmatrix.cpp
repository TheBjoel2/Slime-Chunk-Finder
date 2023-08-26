#include "SeedMatrix.cpp"
#include <iostream>
#include <chrono>

template<typename Callable>
void benchmark(Callable&& callable)
{
    const auto start = std::chrono::steady_clock::now();
    callable();
    const auto finish = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration dur = finish-start;
    std::cout << "It took " << dur.count() << "ns" << std::endl;
}

int32_t main()
{
    benchmark([]
    {
        SlimeSeedMatrix<3749998, 3749998> matrix(SlimeSeedMatrixParams(1234, -1874999, -1874999));
    });
    return 0;
}
