#include "SlimeArea.cpp"
#include <iostream>
#include <cstdint>
#include <vector>
#include <future>
#include <mutex>
#include <thread>
#include <optional>
#include <chrono>

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

    const uint32_t ConstantArea = 3749998*2;
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
    static void print(const GlobalSeedMatrix& matrix, const uint32_t matrixX, const uint32_t matrixZ, const uint32_t farmHeight)
    {
        uint32_t chunkCount = 0;
        for(uint32_t j = 0; j < 16; j++)
        {
            for(uint32_t i = 0; i < 16; i++)
            {
                const bool isSlimeChunk = Util::isSlimeChunk(matrix, matrixX+i, matrixZ+j);
                const bool isWithinSphere = Util::isChunkWithinSphere(i, j, farmHeight);
                chunkCount+=isSlimeChunk&isWithinSphere;
                std::cout << (isSlimeChunk&isWithinSphere ? 'S' : ' ');
            }
            std::cout << std::endl;
        }
        std::cout << chunkCount << " chunks in use" << std::endl;
    }
};

template<uint32_t i = 1>
void findBestSlimePlace(const uint32_t farmHeight, const auto& matrix, const auto& area, auto& toReturn)
{
    if(i == farmHeight)
        _findBestSlimePlace<i>(matrix, area, toReturn);
    else if constexpr(i <= 103)
        findBestSlimePlace<i+1>(farmHeight, matrix, area, toReturn);
}

template<uint32_t farmHeight>
void _findBestSlimePlace(const auto& matrix, const auto& area, auto& toReturn)
{
    for(uint32_t j = area.beginZ; j < area.endZ; j++)
    {
        SlimeArea<farmHeight> slimeArea(matrix, area.beginX, j);
        uint32_t i = area.beginX;
        do
        {
            const uint32_t score = slimeArea.getTotalScore();
            if(score > toReturn.score)
            {
                toReturn.x = i;
                toReturn.z = j;
                toReturn.score = score;
            }
            i++;
            if(i >= area.endX) break;

            slimeArea.shiftToPositiveX();
        }
        while(true);
    }
}

class ProgressUpdate
{
public:
    ProgressUpdate(const std::size_t totalTasks):
        m_TotalTasks(totalTasks){}

    void reportTaskDone()
    {
        {
            std::lock_guard lock(m);
            currentTasks++;
            reported = true;
        }
        cv.notify_one();
    }

    void printWorker() noexcept try
    {
        const auto timeOnStart = std::chrono::steady_clock::now();

        while(true)
        {
            std::unique_lock lock(m);
            cv.wait(lock, [&]{ return reported; });

            const auto timePassedSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-timeOnStart);
            const auto speed = (timePassedSeconds.count() == 0 ? 0 : currentTasks/timePassedSeconds.count());
            const auto secondsLeft = (speed == 0 ? 0 : (m_TotalTasks-currentTasks)/speed);

            std::cout << '[' << currentTasks << '/' << m_TotalTasks << "] "
                      << std::setw(2) << std::setfill('0') << secondsLeft/3600 << ':'
                      << std::setw(2) << std::setfill('0') << secondsLeft%3600/60 << ':'
                      << std::setw(2) << std::setfill('0') << secondsLeft%60 << " left" << std::endl;

            reported = false;

            if(currentTasks >= m_TotalTasks) break;
        }
    }
    catch(...){}

private:
    const std::size_t m_TotalTasks;
    std::size_t currentTasks = 0;
    bool reported = false;

    std::mutex m{};
    std::condition_variable cv{};
};

int32_t main()
{
    //Seed input
    uint64_t seed = 0;
    std::cout << "Enter seed\n: ";
    std::cin >> seed;

    //Radius input
    uint32_t radius = 0;
    std::cout << "Enter Radius\n(min=8, max=1874999): ";
    std::cin >> radius;
    if(radius < 8 || radius > 1874999)
    {
        std::cout << "Please enter proper radius value" << std::endl;
        return 1;
    }

    //farm height input
    uint32_t farmHeight = 0;
    std::cout << "Enter farm height\n(min=1, max=103): ";
    std::cin >> farmHeight;
    if(farmHeight < 1 || farmHeight > 103)
    {
        std::cout << "Please enter proper farm height value" << std::endl;
        return 1;
    }

    //Thread number input
    const uint32_t hardwareThreadNum = std::thread::hardware_concurrency();
    uint32_t threadNum = 0;
    std::cout << "Enter thread number\n(suggestion=" << hardwareThreadNum << "): ";
    std::cin >> threadNum;
    if(threadNum == 0)
    {
        std::cout << "Please enter proper thread number" << std::endl;
        return 1;
    }

    //vector of tasks
    auto areas = getTaskAreasFromUserInput(radius);
    std::mutex areaMutex; //tasks are used by multiple threads

    GlobalSeedMatrix matrix(seed, radius*2);

    //progress output
    ProgressUpdate update(areas.size());

    //time measurement
    const auto timeStarted = std::chrono::steady_clock::now();

    struct SlimePosition
    {
        uint32_t x;
        uint32_t z;
        uint32_t score;
    };
    std::vector<std::future<SlimePosition>> finderFutures; //threads return slime positions
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
                findBestSlimePlace(farmHeight, matrix, area, toReturn);

                //reporting
                update.reportTaskDone();

                lock.lock(); //the next access is areas.empty(), that's why locking
            }

            return toReturn;
        }));

    auto updateFuture = std::async(&ProgressUpdate::printWorker, &update);

    //processing threads output
    std::vector<SlimePosition> positions;
    for(auto& fut : finderFutures)
        positions.emplace_back(fut.get());

    //wait for progress update finishing printing
    updateFuture.wait();

    //time measurement
    const auto timeEnded = std::chrono::steady_clock::now();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeEnded-timeStarted);
    std::cout << "It took " << seconds.count() << " seconds" << std::endl;

    const auto bestPosition = std::max_element(positions.cbegin(), positions.cend(), [](const auto& a, const auto& b)->bool
    {
        return a.score < b.score;
    });
    if(bestPosition->score == 0)
    {
        std::cout << "Sorry, no slime chunks for you today :/" << std::endl;
        return 0;
    }

    SlimeChunkPrinter::print(matrix, bestPosition->x, bestPosition->z, farmHeight);
    std::cout << "AFK position at X:"
              << (static_cast<int32_t>(bestPosition->x)-static_cast<int32_t>(radius))*16+128 << "; Z:"
              << (static_cast<int32_t>(bestPosition->z)-static_cast<int32_t>(radius))*16+128 << " (score="
              << bestPosition->score << ")\n"
              << "Higher score means better" << std::endl;
    //do not close right after we found it
    char ch;
    std::cin >> ch;
}
