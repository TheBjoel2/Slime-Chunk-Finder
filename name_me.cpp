#include "GlobalSeedMatrix.cpp"

struct MatrixPoint
{
    uint32_t x;
    uint32_t z;
};

/*class MatrixIterator
{

};

MatrixIterator iterate(const GlobalSeedMatrix& seedMatrix, const MatrixPoint topLeft, const MatrixPoint bottomRight)
{

}*/

class ShiftingXMatrixIterator
{
public:
    static ShiftingXMatrixIterator iterate(const GlobalSeedMatrix& seedMatrix, const uint32_t beginMatrixX, const uint32_t endMatrixX)
    {
        return ShiftingXMatrixIterator(seedMatrix, beginMatrixX, endMatrixX);
    }

    bool isEnded()
    {
        return currentMatrixX == endMatrixX;
    }

    void operator++()
    {
        currentXValue = seedMatrix.getXValue(++currentMatrixX);
    }

    uint64_t getSeedValue(const uint32_t matrixZ)
    {
        return (currentXValue+seedMatrix.getZValue(matrixZ))^0x3ad8025f;
    }

private:
    const GlobalSeedMatrix& seedMatrix;
    uint32_t currentMatrixX;
    const uint32_t endMatrixX;
    uint32_t currentXValue;

    ShiftingXMatrixIterator(const GlobalSeedMatrix& seedMatrix, const uint32_t currentMatrixX, const uint32_t endMatrixX):
        seedMatrix(seedMatrix),
        currentMatrixX(currentMatrixX),
        endMatrixX(endMatrixX),
        currentXValue(seedMatrix.getXValue(currentMatrixX)){}
};
