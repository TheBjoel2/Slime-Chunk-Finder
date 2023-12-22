#include "GlobalSeedMatrix.cpp"
#include "JavaRandom.cpp"

namespace Util
{

/*We only work with matrix, that's why matrix X and Z*/
bool isSlimeChunk(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ)
{
    const uint32_t randValue = JavaRandom::nextIntForSeed(matrix.getSeedValue(matrixX, matrixZ), 10);
    return (randValue == 0);
}

}
