#include "SeedMatrix.cpp"
#include "StackMatrix.cpp"
#include "JavaRandom.cpp"
#include "Util.cpp"

class SlimeArea
{
public:
    SlimeArea(const SlimeSeedMatrix& slimeSeedMatrix, const uint32_t matrixX, const uint32_t matrixZ):
        slimeSeedMatrix(slimeSeedMatrix),
        currentMatrixX(matrixX),
        currentMatrixZ(matrixZ)
    {
        for(uint8_t j = 0; j < 15; j++)
            for(uint8_t i = 0; i < 15; i++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(slimeSeedMatrix, matrixX+i, matrixZ+j);
                areaStorage.get(i, j) = isSlimeChunk;
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

    StackMatrix<bool, 15, 15> areaStorage{};
    uint16_t circleAreaSum = 0; //note that the areaStorage is a matrix -> is a square. But only the area in a circle range is being calculated
    uint8_t storageColumnShiftIndex = 0;

    uint32_t currentMatrixX;
    uint32_t currentMatrixZ;

    uint8_t getNormalizedIndex(const uint8_t index)
    {
        if(index >= 15)
            return index-15;
        return index;
    }

    void calculateNewColumn()
    {
        for(uint8_t j = 0; j < 15; j++)
        {
            const bool isSlimeChunk = Util::isSlimeChunk(slimeSeedMatrix, currentMatrixX+15, currentMatrixZ+j);
            areaStorage.get(storageColumnShiftIndex, j) = isSlimeChunk;
        }
        currentMatrixX++;
        storageColumnShiftIndex = getNormalizedIndex(storageColumnShiftIndex+1);
    }

    auto getFromStorageWithShiftedIndex(const uint8_t x, const uint8_t y)
    {
        return areaStorage.get(getNormalizedIndex(x+storageColumnShiftIndex), y);
    }

    void substractFromSum()
    {
        //substracting. literally hardcoded coordinates yeah
        if(getFromStorageWithShiftedIndex(4, 0))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(2, 1))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(1, 2))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(1, 3))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 4))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 5))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 6))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 7))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 8))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 9))  circleAreaSum--;
        if(getFromStorageWithShiftedIndex(0, 10)) circleAreaSum--;
        if(getFromStorageWithShiftedIndex(1, 11)) circleAreaSum--;
        if(getFromStorageWithShiftedIndex(1, 12)) circleAreaSum--;
        if(getFromStorageWithShiftedIndex(2, 13)) circleAreaSum--;
        if(getFromStorageWithShiftedIndex(4, 14)) circleAreaSum--;
    }

    void addToSum()
    {
/*         hardcode goes brr
 *         I honestly thought of calculating those indexes in constexpr context. There are two reasons why it hasn't been implemented
 *         1. I'm skill issue
 *         2. Not worth it. Figuring out these by hand is easy
 */
        if(getFromStorageWithShiftedIndex(11, 0))  circleAreaSum++;
        if(getFromStorageWithShiftedIndex(13, 1))  circleAreaSum++;
        if(getFromStorageWithShiftedIndex(14, 2))  circleAreaSum++;
        if(getFromStorageWithShiftedIndex(14, 3))  circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 4))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 5))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 6))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 7))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 8))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 9))   circleAreaSum++;
        if(getFromStorageWithShiftedIndex(0, 10))  circleAreaSum++;
        if(getFromStorageWithShiftedIndex(14, 11)) circleAreaSum++;
        if(getFromStorageWithShiftedIndex(14, 12)) circleAreaSum++;
        if(getFromStorageWithShiftedIndex(13, 13)) circleAreaSum++;
        if(getFromStorageWithShiftedIndex(11, 14)) circleAreaSum++;
    }
};
