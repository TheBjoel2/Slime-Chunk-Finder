#include "SeedMatrix.cpp"
#include "JavaRandom.cpp"
#include "Util.cpp"
#include <bitset>

class SlimeArea
{
public:
    SlimeArea(const SlimeSeedMatrix& slimeSeedMatrix, const uint32_t matrixX, const uint32_t matrixZ):
        slimeSeedMatrix(slimeSeedMatrix),
        currentMatrixX(matrixX),
        currentMatrixZ(matrixZ)
    {
        for(uint8_t i = 0; i < 15; i++)
            for(uint8_t j = 0; j < 15; j++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(slimeSeedMatrix, matrixX+i, matrixZ+j);
                areaStorage[i*15+j] = isSlimeChunk; //maybe optimize to i<<4 ????
                circleAreaSum+=(isSlimeChunk && Util::isChunkInRangeCornerOrigin(i, j));
            }
    }

    uint16_t getAreaSum()
    {
        return circleAreaSum;
    }

    void shiftToPositiveX()
    {
        substractFromSum();
        calculateNewColumn();
        addToSum();
    }

private:
    const SlimeSeedMatrix& slimeSeedMatrix;

/*     areaStorage bit order:
 *     giving that I is parallel to chunkX, J is parallel to chunkZ
 *     the order is (i0, j0)...(i0, j14)...(i14, j14)
 */
    std::bitset<15*15> areaStorage{};
    uint16_t circleAreaSum = 0; //note that the areaStorage is a matrix, stored in a bitset -> is a square. But only the area in a circle range is being calculated

    uint32_t currentMatrixX;
    uint32_t currentMatrixZ;

    uint8_t storageShiftIndex = 0;

    uint8_t getNormalizedIndex(const uint16_t index)
    {
        if(index >= 15*15)
            return index-15*15;
        return index;
    }

    auto accessStorageWithShiftedIndex(const uint8_t i, const uint8_t j)
    {
        return areaStorage[getNormalizedIndex(i*15+j+storageShiftIndex)];
    }

    void calculateNewColumn()
    {
        for(uint8_t j = 0; j < 15; j++)
        {
            const bool isSlimeChunk = Util::isSlimeChunk(slimeSeedMatrix, currentMatrixX+15, currentMatrixZ+j);
            accessStorageWithShiftedIndex(0, j) = isSlimeChunk;
        }
        currentMatrixX++;
        storageShiftIndex = getNormalizedIndex(storageShiftIndex+15);
    }

    void substractFromSum()
    {
        //substracting. literally hardcoded coordinates yeah
        if(accessStorageWithShiftedIndex(4, 0))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(2, 1))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(1, 2))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(1, 3))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 4))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 5))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 6))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 7))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 8))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 9))  circleAreaSum--;
        if(accessStorageWithShiftedIndex(0, 10)) circleAreaSum--;
        if(accessStorageWithShiftedIndex(1, 11)) circleAreaSum--;
        if(accessStorageWithShiftedIndex(1, 12)) circleAreaSum--;
        if(accessStorageWithShiftedIndex(2, 13)) circleAreaSum--;
        if(accessStorageWithShiftedIndex(4, 14)) circleAreaSum--;
    }

    void addToSum()
    {
/*         hardcode goes brr
 *         I honestly thought of calculating those indexes in constexpr context. There are two reasons why it hasn't been implemented
 *         1. I'm skill issue
 *         2. Not worth it. Figuring out these by hand is easy
 */
        if(accessStorageWithShiftedIndex(11, 0))  circleAreaSum++;
        if(accessStorageWithShiftedIndex(13, 1))  circleAreaSum++;
        if(accessStorageWithShiftedIndex(14, 2))  circleAreaSum++;
        if(accessStorageWithShiftedIndex(14, 3))  circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 4))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 5))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 6))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 7))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 8))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 9))   circleAreaSum++;
        if(accessStorageWithShiftedIndex(0, 10))  circleAreaSum++;
        if(accessStorageWithShiftedIndex(14, 11)) circleAreaSum++;
        if(accessStorageWithShiftedIndex(14, 12)) circleAreaSum++;
        if(accessStorageWithShiftedIndex(13, 13)) circleAreaSum++;
        if(accessStorageWithShiftedIndex(11, 14)) circleAreaSum++;
    }
};
