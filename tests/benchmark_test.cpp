#include "SeedMatrix.cpp"
#include "JavaRandom.cpp"
#include <iostream>
#include <chrono>
#include <cassert>

template<typename Callable>
void benchmark(Callable&& callable)
{
    const auto start = std::chrono::steady_clock::now();
    callable();
    const auto finish = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration dur = finish-start;
    std::cout << "It took " << dur.count() << "ns" << std::endl;
}

uint64_t runMatrix()
{
    SlimeSeedMatrixParams params;
    params.worldSeed = 123456789;
    params.chunkPosX = 82642;
    params.chunkPosZ = -21721;

    SlimeSeedMatrix<1024, 1024> matrix(params);

    uint64_t toReturn = 0;
    for(uint16_t j = 0; j < 1024; j++)
        for(uint16_t i = 0; i < 1024; i++)
            toReturn+=matrix.getSeedValue(i, j);

    return toReturn;
}

uint64_t getSeed(uint64_t seed, uint32_t x, uint32_t z)
{
    const uint64_t a = (uint64_t)(int32_t)(x*x*4987142U) + (uint64_t)(int32_t)(x*5947611U);
    const uint64_t b = (uint64_t)(int32_t)(z*z)*4392871ULL + (uint64_t)(int32_t)(z*389711U);
    return seed+a+b^987234911ULL;
}

uint64_t runLegacy()
{
    uint64_t toReturn = 0;
    for(uint16_t j = 0; j < 1024; j++)
        for(uint16_t i = 0; i < 1024; i++)
            toReturn+=getSeed(123456789, 82642+i, -21721+j);

    return toReturn;
}

uint64_t runGenLegacy()
{
    uint64_t toReturn = 0;
    for(uint16_t j = 0; j < 1024; j++)
        for(uint16_t i = 0; i < 1024; i++)
            toReturn+=JavaRandom::nextIntForSeed(getSeed(123456789, 82642+i, -21721+j), 10);

    return toReturn;
}

uint64_t runGenMatrix()
{
    SlimeSeedMatrixParams params;
    params.worldSeed = 123456789;
    params.chunkPosX = 82642;
    params.chunkPosZ = -21721;

    SlimeSeedMatrix<1024, 1024> matrix(params);

    uint64_t toReturn = 0;
    for(uint16_t j = 0; j < 1024; j++)
        for(uint16_t i = 0; i < 1024; i++)
            toReturn+=JavaRandom::nextIntForSeed(matrix.getSeedValue(i, j), 10);

    return toReturn;
}

int32_t main()
{
    assert(runLegacy() == runMatrix());
    assert(runGenLegacy() == runGenMatrix());

    benchmark(runLegacy);
    benchmark(runMatrix);

    benchmark(runGenLegacy);
    benchmark(runGenMatrix);
}
