#include "GlobalSeedMatrix.cpp"
#include "JavaRandom.cpp"
#include <cmath>

namespace Util
{

/* you know, I'm thinking that passing all these sets of uint32_t and int32_t parameters
 * kinda creates some kind of solyanka, where you never know whether you should pass
 * a coordinate at the range of [0, 16), or [-8, 8), or even a block coordinate.
 * I probably should create separate structures aggregating these values, so that
 * you would never pass a wrong or an unexpected value to it by accident.
 */

/*We only work with matrix, that's why matrix X and Z*/
constexpr bool isSlimeChunk(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ)
{
    const uint32_t randValue = JavaRandom::nextIntForSeed(matrix.getSeedValue(matrixX, matrixZ), 10);
    return (randValue == 0);
}

constexpr uint32_t getChunkVolume(const uint32_t x, const uint32_t z, const uint32_t farmHeight)
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

    const int32_t translatedX = static_cast<int32_t>(x-8);
    const int32_t translatedZ = static_cast<int32_t>(z-8);


    return calculateVolume(translatedX, translatedZ);
}

constexpr bool isChunkWithinSphere(const uint32_t x, const uint32_t z, const uint32_t farmHeight)
{
    return getChunkVolume(x, z, farmHeight) != 0;
}

//(0, 0, 0) is a center of the sphere
//pls do not overflow
constexpr bool isBlockWithinSphere(const int32_t x, const int32_t y, const int32_t z)
{
    const auto sum = x*x+y*y+z*z;
    return sum <= 16384 && sum > 576;
}

//assumes that the player is standing at (0, 0)
//pls do not overflow
constexpr bool isChunkTickableByPlayer(const int32_t chunkX, const int32_t chunkZ)
{
    const int32_t x = chunkX*16+8;
    const int32_t z = chunkZ*16+8;
    const auto sum = x*x+z*z;
    return sum < 16384; //yes, it is '<' not '<='
}

}
