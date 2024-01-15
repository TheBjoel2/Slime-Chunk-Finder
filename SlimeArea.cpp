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
                const bool isWithinSphere = Util::isChunkWithinSphere(i, j);
                storage[j][i] = isSlimeChunk;
                totalScore+=(isSlimeChunk&isWithinSphere ? Util::getChunkScore(i, j) : 0);
            }
    }

    uint32_t getTotalScore() const
    {
        return totalScore;
    }

    void shiftToPositiveX()
    {
        substractFromScore();
        calculateNewColumn();
        addToScore();
        incrementGenerateToX();
    }

private:
    const GlobalSeedMatrix& matrix;
    bool storage[16][16];
    uint32_t totalScore = 0;
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

    void substractFromScore()
    {
        //that's right. it's hardcoded
        if(getFromStorageWithShiftedIndex(4, 0)) totalScore-=755;
        if(getFromStorageWithShiftedIndex(5, 0)) totalScore-=2926;
        if(getFromStorageWithShiftedIndex(6, 0)) totalScore-=2114;
        if(getFromStorageWithShiftedIndex(7, 0)) totalScore-=1039;

        if(getFromStorageWithShiftedIndex(3, 1)) totalScore-=3588;
        if(getFromStorageWithShiftedIndex(4, 1)) totalScore-=3964;
        if(getFromStorageWithShiftedIndex(5, 1)) totalScore-=384;

        if(getFromStorageWithShiftedIndex(2, 2)) totalScore-=4828;
        if(getFromStorageWithShiftedIndex(3, 2)) totalScore-=3108;

        if(getFromStorageWithShiftedIndex(1, 3)) totalScore-=3588;
        if(getFromStorageWithShiftedIndex(2, 3)) totalScore-=4348;

        if(getFromStorageWithShiftedIndex(0, 4)) totalScore-=755;
        if(getFromStorageWithShiftedIndex(1, 4)) totalScore-=6797;
        if(getFromStorageWithShiftedIndex(2, 4)) totalScore-=384;

        if(getFromStorageWithShiftedIndex(0, 5)) totalScore-=3681;
        if(getFromStorageWithShiftedIndex(1, 5)) totalScore-=4255;

        if(getFromStorageWithShiftedIndex(0, 6)) totalScore-=5795;
        if(getFromStorageWithShiftedIndex(1, 6)) totalScore-=2141;

        if(getFromStorageWithShiftedIndex(0, 7)) totalScore-=6834;
        if(getFromStorageWithShiftedIndex(1, 7)) totalScore-=1102;

        if(getFromStorageWithShiftedIndex(0, 8)) totalScore-=6834;
        if(getFromStorageWithShiftedIndex(1, 8)) totalScore-=1102;

        if(getFromStorageWithShiftedIndex(0, 9)) totalScore-=5795;
        if(getFromStorageWithShiftedIndex(1, 9)) totalScore-=2141;

        if(getFromStorageWithShiftedIndex(0, 10)) totalScore-=3681;
        if(getFromStorageWithShiftedIndex(1, 10)) totalScore-=4255;

        if(getFromStorageWithShiftedIndex(0, 11)) totalScore-=755;
        if(getFromStorageWithShiftedIndex(1, 11)) totalScore-=6797;
        if(getFromStorageWithShiftedIndex(2, 11)) totalScore-=384;

        if(getFromStorageWithShiftedIndex(1, 12)) totalScore-=3588;
        if(getFromStorageWithShiftedIndex(2, 12)) totalScore-=4348;

        if(getFromStorageWithShiftedIndex(2, 13)) totalScore-=4828;
        if(getFromStorageWithShiftedIndex(3, 13)) totalScore-=3108;

        if(getFromStorageWithShiftedIndex(3, 14)) totalScore-=3588;
        if(getFromStorageWithShiftedIndex(4, 14)) totalScore-=3964;
        if(getFromStorageWithShiftedIndex(5, 14)) totalScore-=384;

        if(getFromStorageWithShiftedIndex(4, 15)) totalScore-=755;
        if(getFromStorageWithShiftedIndex(5, 15)) totalScore-=2926;
        if(getFromStorageWithShiftedIndex(6, 15)) totalScore-=2114;
        if(getFromStorageWithShiftedIndex(7, 15)) totalScore-=1039;

        //that little motherfucker sphere
        if(getFromStorageWithShiftedIndex(7, 6)) totalScore+=855;

        if(getFromStorageWithShiftedIndex(6, 7)) totalScore+=855;
        if(getFromStorageWithShiftedIndex(7, 7)) totalScore+=4003;

        if(getFromStorageWithShiftedIndex(6, 8)) totalScore+=855;
        if(getFromStorageWithShiftedIndex(7, 8)) totalScore+=4003;

        if(getFromStorageWithShiftedIndex(7, 9)) totalScore+=855;
    }

    void addToScore()
    {
        if(getFromStorageWithShiftedIndex(9, 0)) totalScore+=1039;
        if(getFromStorageWithShiftedIndex(10, 0)) totalScore+=2114;
        if(getFromStorageWithShiftedIndex(11, 0)) totalScore+=2926;
        if(getFromStorageWithShiftedIndex(12, 0)) totalScore+=755;

        if(getFromStorageWithShiftedIndex(11, 1)) totalScore+=384;
        if(getFromStorageWithShiftedIndex(12, 1)) totalScore+=3964;
        if(getFromStorageWithShiftedIndex(13, 1)) totalScore+=3588;

        if(getFromStorageWithShiftedIndex(13, 2)) totalScore+=3108;
        if(getFromStorageWithShiftedIndex(14, 2)) totalScore+=4828;

        if(getFromStorageWithShiftedIndex(14, 3)) totalScore+=4348;
        if(getFromStorageWithShiftedIndex(15, 3)) totalScore+=3588;

        if(getFromStorageWithShiftedIndex(14, 4)) totalScore+=384;
        if(getFromStorageWithShiftedIndex(15, 4)) totalScore+=6797;
        if(getFromStorageWithShiftedIndex(0, 4)) totalScore+=755;

        if(getFromStorageWithShiftedIndex(15, 5)) totalScore+=4255;
        if(getFromStorageWithShiftedIndex(0, 5)) totalScore+=3681;

        if(getFromStorageWithShiftedIndex(15, 6)) totalScore+=2141;
        if(getFromStorageWithShiftedIndex(0, 6)) totalScore+=5795;

        if(getFromStorageWithShiftedIndex(15, 7)) totalScore+=1102;
        if(getFromStorageWithShiftedIndex(0, 7)) totalScore+=6834;

        if(getFromStorageWithShiftedIndex(15, 8)) totalScore+=1102;
        if(getFromStorageWithShiftedIndex(0, 8)) totalScore+=6834;

        if(getFromStorageWithShiftedIndex(15, 9)) totalScore+=2141;
        if(getFromStorageWithShiftedIndex(0, 9)) totalScore+=5795;

        if(getFromStorageWithShiftedIndex(15, 10)) totalScore+=4255;
        if(getFromStorageWithShiftedIndex(0, 10)) totalScore+=3681;

        if(getFromStorageWithShiftedIndex(14, 11)) totalScore+=384;
        if(getFromStorageWithShiftedIndex(15, 11)) totalScore+=6797;
        if(getFromStorageWithShiftedIndex(0, 11)) totalScore+=755;

        if(getFromStorageWithShiftedIndex(14, 12)) totalScore+=4348;
        if(getFromStorageWithShiftedIndex(15, 12)) totalScore+=3588;

        if(getFromStorageWithShiftedIndex(13, 13)) totalScore+=3108;
        if(getFromStorageWithShiftedIndex(14, 13)) totalScore+=4828;

        if(getFromStorageWithShiftedIndex(11, 14)) totalScore+=384;
        if(getFromStorageWithShiftedIndex(12, 14)) totalScore+=3964;
        if(getFromStorageWithShiftedIndex(13, 14)) totalScore+=3588;

        if(getFromStorageWithShiftedIndex(9, 15)) totalScore+=1039;
        if(getFromStorageWithShiftedIndex(10, 15)) totalScore+=2114;
        if(getFromStorageWithShiftedIndex(11, 15)) totalScore+=2926;
        if(getFromStorageWithShiftedIndex(12, 15)) totalScore+=755;

        //that little motherfucker sphere
        if(getFromStorageWithShiftedIndex(9, 6)) totalScore-=855;

        if(getFromStorageWithShiftedIndex(9, 7)) totalScore-=4003;
        if(getFromStorageWithShiftedIndex(10, 7)) totalScore-=855;

        if(getFromStorageWithShiftedIndex(9, 8)) totalScore-=4003;
        if(getFromStorageWithShiftedIndex(10, 8)) totalScore-=855;

        if(getFromStorageWithShiftedIndex(9, 9)) totalScore-=855;
    }
};
