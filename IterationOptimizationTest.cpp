/* Кароч идея такая. Вот достаю я значит значение из двух векторов, да. Чтобы потом сложить, да.
 * А теперь вопрос, а компилятор оптимизирует? Типо я на каждой какой-то итерации могу делать дереференс,
 * но по факту его нужно делать раз в 15 итераций. Или компилятор не тупой как биологическая женщина. */

#include "GlobalSeedMatrix.cpp"
#include "ShiftingXMatrixIteration.cpp"
#include <iostream>
#include <chrono>
#include <experimental/simd>

namespace stdx = std::experimental;

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
    const uint64_t worldSeed = 1234;
    const uint32_t worldChunkSize = 3749998;
    GlobalSeedMatrix matrix(worldSeed, worldChunkSize);

    int32_t posX, posZ;
    std::cin >> posX >> posZ;

    uint64_t answer = 0;
    const auto simpleIteration = [&]
    {
        for(uint32_t i = posX+worldChunkSize/2; i < worldChunkSize; i++)
            for(uint32_t j = posZ+worldChunkSize/2; j < worldChunkSize && j < posZ+worldChunkSize/2+32; j++)
                answer+=matrix.getSeedValue(i, j);
    };
    benchmark(simpleIteration);
    std::cout << answer << std::endl;

    answer = 0;
    const auto optimizedIteration = [&]
    {
        for(uint32_t i = posX+worldChunkSize/2; i < worldChunkSize; i++)
        {
            const uint64_t xValue = matrix.getXValue(i);
            for(uint32_t j = posZ+worldChunkSize/2; j < worldChunkSize && j < posZ+worldChunkSize/2+32; j++)
                answer+=xValue+matrix.getZValue(j);
        }
    };
    benchmark(optimizedIteration);
    std::cout << answer << std::endl;

    answer = 0;
    const auto customIterator = [&]
    {
        for(const auto xValue : ShiftingXMatrixIteration(matrix, posX+worldChunkSize/2, worldChunkSize))
            for(uint32_t j = posZ+worldChunkSize/2; j < worldChunkSize && j < posZ+worldChunkSize/2+32; j++)
                answer+=xValue+matrix.getZValue(j);
    };
    benchmark(customIterator);
    std::cout << answer << std::endl;

    answer = 0;
    const auto customIterator2 = [&] //faster
    {
        for(const auto xValue : ShiftingXMatrixIteration(matrix, posX+worldChunkSize/2, worldChunkSize))
        {
            stdx::fixed_size_simd<uint64_t, 32> val([&](uint32_t j)
            {
                return matrix.getZValue(j+posZ+worldChunkSize/2);
            });
            for(uint32_t j = 0; j < 32; j++)
                answer+=xValue+val[j];
        }
    };
    benchmark(customIterator2);
    std::cout << answer << std::endl;
}

/*Вывод: есть небольшой смысл делать эту оптимизацию*/
/*Вывод2: оверхеда у итератора нема*/
