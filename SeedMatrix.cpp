#ifndef SEED_MATRIX_CPP
#define SEED_MATRIX_CPP

#include <cstdint>
#include <vector>

struct SlimeSeedMatrixParams
{
    uint64_t worldSeed;
    int32_t chunkPosX;
    int32_t chunkPosZ;
    uint32_t width;
    uint32_t height;
};

/* See https://minecraft.fandom.com/wiki/Slime to understand what's happening with those calculations
 * C equivalent would be this:
 * seed + (uint64_t)(int32_t)(x*x*4987142U) + (uint64_t)(int32_t)(x*5947611U) + (uint64_t)(int32_t)(z*z)*4392871ULL + (uint64_t)(int32_t)(z*389711U) ^ 987234911ULL
 */
class SlimeSeedMatrix
{
public:
    SlimeSeedMatrix(const SlimeSeedMatrixParams& params):
        params(params)
    {
        XPlusSeedPart.reserve(params.width);
                ZPart.reserve(params.height);
        calculateXValues(params);
        calculateZValues(params);
    }

    //if not clear, X and Z coordinates in the matrix, not the coordinates of a chunk in a world
    uint64_t getSeedValue(const uint32_t matrixX, const uint32_t matrixZ) const
    {
        return (XPlusSeedPart[matrixX]+ZPart[matrixZ])^0x3ad8025f;
    }

    uint64_t getXValue(const uint32_t matrixX) { return XPlusSeedPart[matrixX]; }
    uint64_t getZValue(const uint32_t matrixZ) { return ZPart[matrixZ]; }

    const SlimeSeedMatrixParams& getParams() { return params; }

private:
    SlimeSeedMatrixParams params;
    std::vector<uint64_t> XPlusSeedPart{};
    std::vector<uint64_t>         ZPart{};

    void calculateXValues(SlimeSeedMatrixParams params)
    {
        for(uint32_t x = params.chunkPosX;
            static_cast<int32_t>(x) < static_cast<int32_t>(params.chunkPosX+params.width);
            x++)
        {
            const uint64_t a = static_cast<int32_t>(x*x*0x4c1906);
            const uint64_t b = static_cast<int32_t>(x*0x5ac0db);
            XPlusSeedPart.push_back(a+b+params.worldSeed);
        }
    }
    void calculateZValues(SlimeSeedMatrixParams params)
    {
        for(uint32_t z = params.chunkPosZ;
            static_cast<int32_t>(z) < static_cast<int32_t>(params.chunkPosZ+params.height);
            z++)
        {
            const uint64_t a = static_cast<int32_t>(z*z)*static_cast<uint64_t>(0x4307a7);
            const uint64_t b = static_cast<int32_t>(z*0x5f24f);
            ZPart.push_back(a+b);
        }
    }
};

#endif // SEED_MATRIX_CPP
