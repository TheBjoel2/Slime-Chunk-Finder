#include "GlobalSeedMatrix.cpp"

//iteration strategy
class ShiftingXMatrixIteration
{
public:
    class Iterator
    {
    public:
        uint64_t operator*() const { return currentXValue; }
        void operator++() { currentMatrixX++; currentXValue = seedMatrix.getXValue(currentMatrixX); }
        bool operator!=(const Iterator& iter) const { return currentMatrixX != iter.currentMatrixX; }

    private:
        const GlobalSeedMatrix& seedMatrix;
        uint32_t currentMatrixX;
        uint64_t currentXValue;

        Iterator(const GlobalSeedMatrix& seedMatrix, const uint32_t currentMatrixX):
            seedMatrix(seedMatrix),
            currentMatrixX(currentMatrixX),
            currentXValue(seedMatrix.getXValue(currentMatrixX)){}

        friend class ShiftingXMatrixIteration; //in order to get access to the constructor
    };

public:
    ShiftingXMatrixIteration(const GlobalSeedMatrix& seedMatrix, const uint32_t beginMatrixX, const uint32_t endMatrixX):
        seedMatrix(seedMatrix),
        beginMatrixX(beginMatrixX),
        endMatrixX(endMatrixX){}

    Iterator begin() const { return Iterator(seedMatrix, beginMatrixX); }
    Iterator end() const { return Iterator(seedMatrix, endMatrixX); }

private:
    const GlobalSeedMatrix& seedMatrix;
    const uint32_t beginMatrixX;
    const uint32_t endMatrixX;
};
