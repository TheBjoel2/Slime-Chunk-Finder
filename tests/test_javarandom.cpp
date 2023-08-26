#include "SeedMatrix.cpp"
#include "JavaRandom.cpp"
#include <iostream>

int32_t main()
{
    SlimeSeedMatrixParams params;
    params.worldSeed = 123456789;
    params.chunkPosX = 82642;
    params.chunkPosZ = -21721;

    SlimeSeedMatrix<256, 256> matrix(params);
    std::cout << JavaRandom::nextIntForSeed(matrix.getSeedValue(7, 110), 10) << '\n';
    std::cout << JavaRandom::nextIntForSeed(matrix.getSeedValue(200, 0), 10) << '\n';
    std::cout << JavaRandom::nextIntForSeed(matrix.getSeedValue(32, 52), 10) << std::endl;
}
