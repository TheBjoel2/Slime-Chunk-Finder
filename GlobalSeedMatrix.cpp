#include "SeedMatrix.cpp"

//I believe that the world size in chunks is 3749998*3749998
//Top corner chunk of a world should be at X:-1874999 Z:-1874999

class GlobalSeedMatrix : public SlimeSeedMatrix
{
public:
    GlobalSeedMatrix(const uint64_t seed, const uint32_t width):
        SlimeSeedMatrix(SlimeSeedMatrixParams{seed, -static_cast<int32_t>(width/2), -static_cast<int32_t>(width/2), width, width}){}
};
