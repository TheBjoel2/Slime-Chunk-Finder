#include <cstdint>

class JavaRandom
{
private:
    uint64_t m_Seed;

    uint32_t next(const uint32_t bits)
    {
        m_Seed = (m_Seed*0x5deece66d+0xb)&0xffffffffffff;
        return m_Seed>>(48-bits);
    }
public:
    uint32_t nextInt(const uint32_t bound)
    {
        uint32_t r = next(31);
        const uint32_t m = bound-1;

        //this check will be thrown out by the compiler since bound=10
        if((bound&m) == 0)
            r = (bound*static_cast<uint64_t>(r))>>31;
        else for(uint32_t u = r; static_cast<int32_t>(u-(r = u%bound)+m) < 0; u = next(31));

        return r;
    }

    void setSeed(const uint64_t seed)
    {
        m_Seed = (seed^0x5deece66d)&0xffffffffffff;
    }

    void setRawSeed(const uint64_t seed)
    {
        m_Seed = seed;
    }

    static uint32_t nextIntForSeed(const uint64_t seed, const uint32_t bound)
    {
        JavaRandom rand;
        rand.setSeed(seed);
        return rand.nextInt(bound);
    }

    static uint32_t nextIntForRawSeed(const uint64_t seed, const uint32_t bound)
    {
        JavaRandom rand;
        rand.setRawSeed(seed);
        return rand.nextInt(bound);
    }
};
