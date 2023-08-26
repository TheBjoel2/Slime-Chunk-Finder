/* Кароч идея такая. Вот достаю я значит значение из двух векторов, да. Чтобы потом сложить, да.
 * А теперь вопрос, а компилятор оптимизирует? Типо я на каждой какой-то итерации могу делать дереференс,
 * но по факту его нужно делать раз в 15 итераций. Или компилятор не тупой как биологическая женщина. */

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
    const uint64_t worldSeed = 1234;
    const uint32_t worldChunkSize = 3749998;
    GlobalSeedMatrix matrix(worldSeed, worldChunkSize);

    int32_t posX, posZ;
    std::cin >> posX >> posZ;

    uint64_t answer = 0;
    const auto simpleIteration = [&]
    {
        for(uint32_t i = posX+worldChunkSize/2; i < worldChunkSize; i++)
            for(uint32_t j = posZ+worldChunkSize/2; j < worldChunkSize && j < posZ+worldChunkSize/2+15; j++)
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
            for(uint32_t j = posZ+worldChunkSize/2; j < worldChunkSize && j < posZ+worldChunkSize/2+15; j++)
                answer+=((xValue+matrix.getZValue(j))^0x3ad8025f);
        }
    };
    benchmark(optimizedIteration);
    std::cout << answer << std::endl;
}

/*Вывод: есть небольшой смысл делать эту оптимизацию*/
