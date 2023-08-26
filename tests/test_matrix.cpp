#include "SeedMatrix.cpp"
#include <iostream>

int32_t main()
{
    SlimeSeedMatrixParams params;
    params.worldSeed = 123456789;
    params.chunkPosX = 82642;
    params.chunkPosZ = -21721;
    //params.worldSeed = 1;
    //params.chunkPosX = 1;
    //params.chunkPosZ = -1;

    SlimeSeedMatrix<256, 256> matrix(params);
    std::cout << matrix.getSeedValue(7, 110) << '\n';
    std::cout << matrix.getSeedValue(200, 0) << '\n';
    std::cout << matrix.getSeedValue(32, 52) << std::endl;
}

/*uint64_t getSeed(uint64_t seed, uint32_t x, uint32_t z)
{
    const uint64_t a = (uint64_t)(int32_t)(x*x*4987142U) + (uint64_t)(int32_t)(x*5947611U);
    const uint64_t b = (uint64_t)(int32_t)(z*z)*4392871ULL + (uint64_t)(int32_t)(z*389711U);
    return seed+a+b^987234911ULL;
    //return (seed + (uint64_t)(int32_t)(x*x*4987142U) + (uint64_t)(int32_t)(x*5947611U) + (uint64_t)(int32_t)(z*z)*4392871ULL + (uint64_t)(int32_t)(z*389711U)^987234911ULL);
}

int32_t main()
{
    std::cout << getSeed(123456789, 82642+7, -21721+110) << std::endl;
    std::cout << getSeed(123456789, 82642+200, -21721+0) << std::endl;
    std::cout << getSeed(123456789, 82642+32, -21721+52) << std::endl;

    //std::cout << getSeed(1, 1+7, -1+110) << std::endl;
    //std::cout << getSeed(1, 1+200, -1+0) << std::endl;
    //std::cout << getSeed(1, 1+32, -1+52) << std::endl;

    //std::cout << getSeed(123456789, 82642, -21721) << std::endl;
    //std::cout << getSeed(123456789, 82642, -21721) << std::endl;
    //std::cout << getSeed(123456789, 82642, -21721) << std::endl;
}*/
