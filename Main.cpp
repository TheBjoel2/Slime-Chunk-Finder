#include "SlimeArea.cpp"
#include <iostream>
#include <cstdint>
#include <vector>
#include <future>
#include <mutex>

auto getTaskAreasFromUserInput(const uint32_t radius)
{
    struct Area
    {
        uint32_t beginX;
        uint32_t beginZ;
        uint32_t endX;
        uint32_t endZ;
    };
    std::vector<Area> areas;

    const uint32_t ConstantArea = 3749998*10;
    const uint32_t ZStep = ConstantArea/radius;

    uint32_t currentZ = 0;
    do
    {
        Area area;
        area.beginX = 0;
        area.beginZ = currentZ;

        currentZ+=ZStep;
        if(currentZ > radius*2-15)
            currentZ = radius*2-15;

        area.endX = radius*2-15;
        area.endZ = currentZ;

        areas.push_back(area);
    }
    while(currentZ < radius*2-15);

    return areas;
}

class SlimeChunkPrinter
{
public:
    static void print(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ)
    {
        for(uint32_t j = 0; j < 16; j++)
        {
            std::cout << std::endl;
            for(uint32_t i = 0; i < 16; i++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(matrix, matrixX+i, matrixZ+j);
                const bool isWithinSphere = isChunkWithinSphere(i, j);
                std::cout << (isSlimeChunk&isWithinSphere ? 'S' : ' ');
            }
        }
        std::cout << std::endl;
    }

private:
    static uint32_t calculateVolume(const int32_t chunkX, const int32_t chunkZ)
    {
        uint32_t volume = 0;

        for(int32_t x = chunkX*16; x < (chunkX+1)*16; x++)
            for(int32_t z = chunkZ*16; z < (chunkZ+1)*16; z++)
                for(int32_t y = 1; y < 32; y++)
                {
                    const auto sum = x*x+y*y+z*z;
                    if(sum <= 16384 && sum > 576)
                        volume++;
                }
        return volume;
    }

    static bool isChunkWithinSphere(const uint32_t chunkX, const uint32_t chunkZ)
    {
        return calculateVolume(static_cast<int32_t>(chunkX)-8, static_cast<int32_t>(chunkZ)-8) > 1500;
    }
};

int32_t main()
{
    //Seed input
    uint64_t seed;
    std::cout << "Enter seed\n: ";
    std::cin >> seed;

    //Radius input
    uint32_t radius;
    std::cout << "Enter Radius\n(min=8, max=1874999): ";
    std::cin >> radius;
    if(radius < 8)
    {
        std::cout << "Please enter proper radius value" << std::endl;
        return 1;
    }
    if(radius > 1874999)
    {
        std::cout << "Please enter proper radius value" << std::endl;
        return 1;
    }

    //Thread number input
    const uint32_t hardwareThreadNum = std::thread::hardware_concurrency();
    uint32_t threadNum;
    std::cout << "Enter thread number\n(suggestion=" << hardwareThreadNum << "): ";
    std::cin >> threadNum;
    if(threadNum == 0)
    {
        std::cout << "Please enter proper thread number" << std::endl;
        return 1;
    }

    auto areas = getTaskAreasFromUserInput(radius);
    std::mutex areaMutex;

    GlobalSeedMatrix matrix(seed, radius*2);
    /*
    std::vector<std::jthread> finderThreads;
    for(uint32_t i = 0; i < threadNum; i++)
        finderThreads.emplace_back([&]()
        {
            std::unique_lock lock(areaMutex);
            while(!areas.empty())
            {
                //getting area task
                const auto area = areas.back();
                areas.pop_back();
                lock.unlock();

                //searching
                for(uint32_t j = area.beginZ; j < area.endZ; j++)
                {
                    SlimeArea slimeArea(matrix, area.beginX, j);
                    uint32_t i = area.beginX;
                    do
                    {
                        const uint32_t circleArea = slimeArea.getAreaSum();
                        if(circleArea > maxCircleArea)

                    }
                    while(true);
                }

                lock.lock(); //the next access is areas.empty(), that's why locking
            }
        });*/
    struct SlimePosition
    {
        uint32_t x;
        uint32_t z;
        uint32_t slimeChunkCount;
    };
    std::vector<std::future<SlimePosition>> finderFutures;
    for(uint32_t i = 0; i < threadNum; i++)
        finderFutures.emplace_back(std::async([&]()->SlimePosition
        {
            SlimePosition toReturn{0, 0, 0};

            std::unique_lock lock(areaMutex);
            while(!areas.empty())
            {
                //getting area task
                const auto area = areas.back();
                areas.pop_back();
                lock.unlock();

                //searching
                for(uint32_t j = area.beginZ; j < area.endZ; j++)
                {
                    SlimeArea slimeArea(matrix, area.beginX, j);
                    uint32_t i = area.beginX;
                    do
                    {
                        const uint32_t circleArea = slimeArea.getAreaSum();
                        if(circleArea > toReturn.slimeChunkCount)
                        {
                            toReturn.x = i;
                            toReturn.z = j;
                            toReturn.slimeChunkCount = circleArea;
                        }
                        i++;
                        if(i >= area.endX) break;

                        slimeArea.shiftToPositiveX();
                    }
                    while(true);
                }

                lock.lock(); //the next access is areas.empty(), that's why locking
            }

            return toReturn;
        }));

    /*for(auto& fut : finderFutures)
    {
        const auto slimePos = fut.get();
        SlimeChunkPrinter::print(matrix, slimePos.x, slimePos.z);
        std::cout << static_cast<int32_t>(slimePos.x)-static_cast<int32_t>(radius) << ' ' << static_cast<int32_t>(slimePos.z)-static_cast<int32_t>(radius) << ' ' << slimePos.slimeChunkCount << std::endl;
    }*/

    SlimePosition bestPosition{0, 0, 0};
    for(auto& fut : finderFutures)
    {
        const auto slimePos = fut.get();
        if(slimePos.slimeChunkCount > bestPosition.slimeChunkCount)
            bestPosition = slimePos;
    }

    if(bestPosition.slimeChunkCount == 0)
    {
        std::cout << "Sorry, no slime chunks for you today :/" << std::endl;
        return 0;
    }
    SlimeChunkPrinter::print(matrix, bestPosition.x, bestPosition.z);
    std::cout << (static_cast<int32_t>(bestPosition.x)-static_cast<int32_t>(radius))*16 << ' '
              << (static_cast<int32_t>(bestPosition.z)-static_cast<int32_t>(radius))*16 << ' '
              << bestPosition.slimeChunkCount << std::endl;
}
