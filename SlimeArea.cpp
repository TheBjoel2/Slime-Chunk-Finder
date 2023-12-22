#include "GlobalSeedMatrix.cpp"
#include "Util.cpp"
#include <experimental/simd>

namespace stdx = std::experimental;

//storage[z][x]

class SlimeArea //silly algorithm to make everything faster
{
public:
    SlimeArea(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ):
        matrix(matrix),
        currentMatrixX(matrixX),
        currentMatrixZ(matrixZ)
    {
        for(uint32_t j = 0; j < 16; j++)
            for(uint32_t i = 0; i < 16; i++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(matrix, matrixX+i, matrixZ+j);
                const bool isWithinSphere = isChunkWithinSphere(i, j);
                storage[j][i] = isSlimeChunk;
                circleAreaSum+=isSlimeChunk&isWithinSphere;
            }
    }

    uint32_t getAreaSum() const
    {
        return circleAreaSum;
    }

    void shiftToPositiveX()
    {
        substractFromSum();
        calculateNewColumn();
        addToSum();
        incrementGenerateToX();
    }

private:
    const GlobalSeedMatrix& matrix;
    bool storage[16][16];
    uint32_t circleAreaSum = 0;
    uint32_t generateToX = 0;

          uint32_t currentMatrixX;
    const uint32_t currentMatrixZ;

    void calculateNewColumn()
    {
        //slime seed
        const uint64_t xValue = matrix.getXValue(currentMatrixX+16);

        stdx::fixed_size_simd<uint64_t, 16> seed([&](const uint32_t j)
        {
            return matrix.getZValue(currentMatrixZ+j);
        });
        seed+=xValue;
        seed^=0x3ad8025f;

        //javarandom seed
        seed^=0x5deece66dULL;
        seed&=0xffffffffffffULL;

        for(uint32_t j = 0; j < 16; j++)
            storage[j][generateToX] = (JavaRandom::nextIntForRawSeed(seed[j], 10) == 0);

        currentMatrixX++;
    }

    void incrementGenerateToX()
    {
        generateToX++;
        generateToX = (generateToX >= 16 ? generateToX-16 : generateToX);
    }

    bool getFromStorageWithShiftedIndex(const uint32_t x, const uint32_t z)
    {
        const uint32_t i = generateToX+x;
        return storage[z][(i >= 16 ? i-16 : i)];
    }

    void substractFromSum()
    {
        //that's right. it's hardcoded
        if(getFromStorageWithShiftedIndex(5, 0)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(3, 1)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(2, 2)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(1, 3)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(1, 4)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 5)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 6)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 7)) circleAreaSum-=1;

        if(getFromStorageWithShiftedIndex(5, 15)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(3, 14)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(2, 13)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(1, 12)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(1, 11)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 10)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 9)) circleAreaSum-=1;
        if(getFromStorageWithShiftedIndex(0, 8)) circleAreaSum-=1;
    }

    void addToSum()
    {
        if(getFromStorageWithShiftedIndex(11, 0)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(13, 1)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(14, 2)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(15, 3)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(15, 4)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 5)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 6)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 7)) circleAreaSum+=1;

        if(getFromStorageWithShiftedIndex(11, 15)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(13, 14)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(14, 13)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(15, 12)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(15, 11)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 10)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 9)) circleAreaSum+=1;
        if(getFromStorageWithShiftedIndex(0, 8)) circleAreaSum+=1;
    }

    uint32_t calculateVolume(const int32_t chunkX, const int32_t chunkZ)
    {
        uint32_t volume = 0;

        for(int32_t x = chunkX*16; x < (chunkX+1)*16; x++)
            for(int32_t z = chunkZ*16; z < (chunkZ+1)*16; z++)
                for(int32_t y = 1; y < 32; y++)
                {
                    const auto sum = x*x+y*y+z*z;
                    if(sum <= 16384 && sum > 576)
                        volume++;
                }
        return volume;
    }

    bool isChunkWithinSphere(const uint32_t chunkX, const uint32_t chunkZ)
    {
        return calculateVolume(static_cast<int32_t>(chunkX)-8, static_cast<int32_t>(chunkZ)-8) > 1500;
    }
};
