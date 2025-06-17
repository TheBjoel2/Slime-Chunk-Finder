#include "cubiomes/generator.h"
#include "cubiomes/util.h"

class RangedBiomeGenerator
{
public:
    RangedBiomeGenerator(int minecraftVersion, uint64_t worldSeed, Range range):
        m_Range(range)
    {
        //api reference: https://github.com/Cubitect/cubiomes#biome-generation-in-a-range
        setupGenerator(&m_Generator, minecraftVersion, 0);
        applySeed(&m_Generator, DIM_OVERWORLD, worldSeed);

        m_BiomeIds = allocCache(&m_Generator, range);
        if(!m_BiomeIds) throw std::bad_alloc();

        genBiomes(&m_Generator, m_BiomeIds, range);
    }

    //I'm too lazy to implement these
    RangedBiomeGenerator(const RangedBiomeGenerator&) = delete;
    RangedBiomeGenerator& operator=(const RangedBiomeGenerator&) = delete;

    int getBiomeId(const std::size_t x,
                   const std::size_t y,
                   const std::size_t z) const
    {
        //they are indexed like this:
        //biomeIds[i_y*r.sx*r.sz + i_z*r.sx + i_x]
        //source: https://github.com/Cubitect/cubiomes#biome-generation-in-a-range

        return m_BiomeIds[y*m_Range.sx*m_Range.sz
                        + z*m_Range.sx
                        + x];
    }

    ~RangedBiomeGenerator()
    {
        free(m_BiomeIds);
    }

private:
    Generator m_Generator{};
    Range m_Range;
    int* m_BiomeIds = nullptr;
};
