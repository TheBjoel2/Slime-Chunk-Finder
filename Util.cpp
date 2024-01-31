#include "GlobalSeedMatrix.cpp"
#include "JavaRandom.cpp"
#include <cmath>

namespace Util
{

/*We only work with matrix, that's why matrix X and Z*/
bool isSlimeChunk(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ)
{
    const uint32_t randValue = JavaRandom::nextIntForSeed(matrix.getSeedValue(matrixX, matrixZ), 10);
    return (randValue == 0);
}

constexpr uint32_t getChunkScore(const uint32_t x, const uint32_t z, const uint32_t farmHeight)
{
    const auto calculateVolume = [&](const int32_t chunkX, const int32_t chunkZ)->uint32_t
    {
        uint32_t volume = 0;

        for(int32_t x = chunkX*16; x < (chunkX+1)*16; x++)
            for(int32_t z = chunkZ*16; z < (chunkZ+1)*16; z++)
                for(int32_t y = 1; y <= static_cast<int32_t>(farmHeight); y++)
                {
                    const auto sum = x*x+y*y+z*z;
                    if(sum <= 16384 && sum > 576)
                        volume++;
                }
        return volume;
    };

    const int32_t translatedX = -std::abs(int32_t(x < 8 ? x-8 : x-7));
    const int32_t translatedZ = -std::abs(int32_t(z < 8 ? z-8 : z-7));

    return calculateVolume(translatedX, translatedZ);
}

bool isChunkWithinSphere(const uint32_t x, const uint32_t z, const uint32_t farmHeight)
{
    return getChunkScore(x, z, farmHeight) != 0;
}

}
