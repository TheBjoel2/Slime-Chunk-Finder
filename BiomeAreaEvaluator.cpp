#include "RangedBiomeGenerator.cpp"
#include "PackSpawnDistribution.cpp"
#include "GlobalSeedMatrix.cpp"
#include "Versions.cpp"

struct BiomeAreaEvaluatorParams
{
    int minecraftVersion;
    uint64_t worldSeed;
    uint32_t radius;
    uint32_t farmHeight;
    uint32_t matrixX;
    uint32_t matrixZ;
    const GlobalSeedMatrix& matrix;
};

class BiomeAreaEvaluator
{
public:
    //takes per-chunk coordinates
    static uint64_t evaluate(const BiomeAreaEvaluatorParams& params)
    {
        //per-block coordinates
        const int32_t beginX = static_cast<int32_t>(params.matrixX*16 - params.radius*16);
        const int32_t beginZ = static_cast<int32_t>(params.matrixZ*16 - params.radius*16);

        const int32_t endX = beginX + 16*16; //number of chunks multiplied by chunk width
        const int32_t endZ = beginZ + 16*16;

        const int32_t beginY = 1 + Version::getMinYAtGivenMCVersion(params.minecraftVersion);
        const int32_t endY = beginY + params.farmHeight;

        Range range;
        range.scale = 1;
        range.x = beginX, range.sx = endX-beginX;
        range.z = beginZ, range.sz = endZ-beginZ;
        range.y = beginY, range.sy = endY-beginY;

        const RangedBiomeGenerator rangedBiomeGenerator(params.minecraftVersion, params.worldSeed, range);

        //I really need to replace it with some kind of a global slime chunk cache
        bool slimeChunkStorage[16][16];
        for(uint32_t j = 0; j < 16; j++)
            for(uint32_t i = 0; i < 16; i++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(params.matrix, params.matrixX+i, params.matrixZ+j);
                slimeChunkStorage[j][i] = isSlimeChunk;
            }

        uint64_t totalScore = 0;

        const auto isBiomeThatSpawnsSlimes = [&rangedBiomeGenerator](const int32_t blockX,
                                                                     const int32_t blockY,
                                                                     const int32_t blockZ)->bool
        {
            const auto biome = rangedBiomeGenerator.getBiomeId(128+blockX, blockY, 128+blockZ);
            switch(biome)
            {
                case mushroom_fields:
                    return false;
                case mushroom_field_shore:
                    return false;
                case deep_dark:
                    return false;
            }
            return true;
        };

        const auto addToSumAccordingToPackSpawnsFromThisBlock = [&isBiomeThatSpawnsSlimes, &slimeChunkStorage, &totalScore]
                                                                    (const int32_t blockX,
                                                                     const int32_t blockY,
                                                                     const int32_t blockZ)
        {
            for(int32_t j = -20; j <= 20; j++)
                for(int32_t i = -20; i <= 20; i++)
                {
                    const int32_t x = blockX+i;
                    const int32_t z = blockZ+j;

                    //mobs only spawn in the 128 block radius sphere
                    const bool isBlockWithinSphere = Util::isBlockWithinSphere(x, blockY, z);
                    if(!isBlockWithinSphere) continue;

                    const uint32_t relativeChunkX = 8+(x>>4);
                    const uint32_t relativeChunkZ = 8+(z>>4);

                    if(relativeChunkX >= 16
                    || relativeChunkZ >= 16) continue; //it happens sometimes, trust. and it is expected

                    const bool isSlimeChunk = slimeChunkStorage[relativeChunkZ][relativeChunkX];
                    const bool isValidBiome = isBiomeThatSpawnsSlimes(x, blockY-1, z);

                    if(isSlimeChunk && isValidBiome)
                        totalScore+=g_PackSpawnDistribution.getValue(20+i, 20+j);
                }
        };

        //minecraft's spawning algorithm is overly nested too
        for(int32_t x = -128; x < 128; x++)
            for(int32_t z = -128; z < 128; z++)
                for(int32_t y = 1; y <= static_cast<int32_t>(params.farmHeight); y++)
                {
                    const int32_t chunkX = x>>4;
                    const int32_t chunkZ = z>>4;

                    if(Util::isChunkTickableByPlayer(chunkX, chunkZ))
                        addToSumAccordingToPackSpawnsFromThisBlock(x, y, z);
                }

        return totalScore;
    }
};

/*#include <iostream>

int main()
{
    const GlobalSeedMatrix matrix(1, 1'000*2);

    BiomeAreaEvaluatorParams baep
    {
        MC_1_17,
        1, //seed
        1000, //world radius in chunks
        1, //farmHeight
        0, 2, //matrix coords
        matrix
    };

    std::cout << BiomeAreaEvaluator::evaluate(baep) << std::endl;

    uint32_t chunkNum = 0;
    for(uint32_t j = 0; j < 16; j++)
        for(uint32_t i = 0; i < 16; i++)
        {
            const bool isSlimeChunk = Util::isSlimeChunk(matrix, 0+i, 2+j);
            chunkNum+=isSlimeChunk ? Util::getChunkScore(i, j, 1) : 0;
        }
    std::cout << chunkNum << std::endl;
}*/