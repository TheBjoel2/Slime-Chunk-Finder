#include <cstdint>
#include <iostream>
#include <experimental/simd>
#include <chrono>

namespace stdx = std::experimental;

void test(uint64_t);

uint64_t scalarGetChunkValue(const uint64_t seed, const int32_t x, const int32_t z)
{
    const uint32_t ux = x;
    const uint32_t uz = z;
    return seed + (uint64_t)(int32_t)(ux*ux*4987142U) + (uint64_t)(int32_t)(ux*5947611U) + (uint64_t)(int32_t)(uz*uz)*4392871ULL + (uint64_t)(int32_t)(uz*389711U)^987234911ULL;
}

template<size_t size>
stdx::fixed_size_simd<uint64_t, size> vectorGetChunkValue(const uint64_t seed, const uint32_t x, const uint32_t z)
{
    stdx::fixed_size_simd<uint64_t, size> vua64 = static_cast<int32_t>(x*x*0x4c1906);
    stdx::fixed_size_simd<uint64_t, size> vub64 = static_cast<int32_t>(x*0x5ac0db);

    stdx::fixed_size_simd<uint32_t, size> vuaux32;
    for(uint8_t i = 0; i < size; i++)
        vuaux32[i] = z+i;

    stdx::fixed_size_simd<int32_t, size> vsc32 = static_simd_cast<int32_t>(vuaux32*vuaux32);
    stdx::fixed_size_simd<uint64_t, size> vuc64 = static_simd_cast<uint64_t>(vsc32);
    vuc64*=0x4307a7;

    stdx::fixed_size_simd<int32_t, size> vsd32 = static_simd_cast<int32_t>(vuaux32*0x5f24f);
    stdx::fixed_size_simd<uint64_t, size> vud64 = static_simd_cast<uint64_t>(vsd32);

    return vua64+vub64+vuc64+vud64+seed^0x3ad8025f;
}

int32_t main()
{
    uint64_t seed;
    std::cin >> seed;

    int32_t x;
    int32_t z;
    std::cin >> x;
    std::cin >> z;

    const auto start = std::chrono::steady_clock::now();

    /*uint64_t result = 0;
    for(uint32_t i = 0; i < 100'000'000; i++)
        for(uint8_t j = 0; j < 17; j++)
            result+=scalarGetChunkValue(seed, x+i, z+j);*/

    uint64_t result = 0;
    for(uint32_t i = 0; i < 100'000'000; i++)
    {
        auto returned = vectorGetChunkValue<16>(seed, x+i, z);
        for(uint8_t j = 0; j < 16; j++)
            result+=returned[j];
    }

    const auto finish = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration dur = finish-start;
    std::cout << dur.count() << "ns\n" << result << std::endl;

    return 0;
}
