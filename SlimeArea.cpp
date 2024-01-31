#include "GlobalSeedMatrix.cpp"
#include "Util.cpp"
#include <experimental/simd>

namespace stdx = std::experimental;

//storage[z][x]

template<uint32_t farmHeight>
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
                const bool isWithinSphere = Util::isChunkWithinSphere(i, j, farmHeight);
                storage[j][i] = isSlimeChunk;
                totalScore+=(isSlimeChunk&isWithinSphere ? Util::getChunkScore(i, j, farmHeight) : 0);
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

    template<uint32_t i = 0>
    void substractFromScore()
    {
        const uint32_t score = Util::getChunkScore(i%8, i/8, farmHeight);
        const uint32_t lastScore = Util::getChunkScore(i%8-1, i/8, farmHeight);
        const int32_t diff = (i%8 == 0 ? score : score-lastScore);

        if constexpr(diff != 0)
            totalScore-=diff*getFromStorageWithShiftedIndex(i%8, i/8);

        if constexpr(i+1 < 128)
            substractFromScore<i+1>();
    }

    template<uint32_t i = 0>
    void addToScore()
    {
        const uint32_t score = Util::getChunkScore(i%8+9, i/8, farmHeight);
        const uint32_t lastScore = Util::getChunkScore(i%8+8, i/8, farmHeight);
        const int32_t diff = score-lastScore;
        const uint32_t I = (i%8+9 == 16 ? 0 : i%8+9);

        if constexpr(diff != 0)
            totalScore-=diff*getFromStorageWithShiftedIndex(I, i/8);

        if constexpr(i+1 < 128)
            addToScore<i+1>();
    }
};
