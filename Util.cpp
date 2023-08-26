#include "SeedMatrix.cpp"

namespace Util
{

/* Logically works with 15x15 area of chunks. Returns wherher the given coordinate is in range.
 * Think of a despawn sphere of a radius 128 blocks. We literally check if the chunk is in that despawn range or not
 */
bool isChunkInRangeCornerOrigin(const uint8_t x, const uint8_t z)
{
    return (x-7)*(x-7)+(z-7)*(z-7) < 64;
}

/*We only work with matrix, that's why matrix X and Z*/
bool isSlimeChunk(const SlimeSeedMatrix& slimeSeedMatrix, const uint32_t matrixX, const uint32_t matrixZ)
{
    const uint32_t randValue = JavaRandom::nextIntForSeed(slimeSeedMatrix.getSeedValue(matrixX, matrixZ), 10);
    return (randValue == 0);
}

}
