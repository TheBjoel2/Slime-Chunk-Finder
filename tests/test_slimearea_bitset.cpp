#include "SlimeArea_bitset.cpp"
#include "GlobalSeedMatrix.cpp"
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
    const GlobalSeedMatrix slimeSeedMatrix(1, 3749998);
    SlimeArea slimeArea(slimeSeedMatrix, 0, 0);
    uint16_t maxChunks = 0;
    benchmark([&]
    {
        for(uint32_t i = 0; i < 3749998-15; i++)
        {
            slimeArea.shiftToPositiveX();
            const uint16_t currentSum = slimeArea.getAreaSum();
            if(currentSum > maxChunks) maxChunks = currentSum;
        }
    });
    std::cout << maxChunks << std::endl;
}
