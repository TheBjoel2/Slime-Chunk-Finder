#include "cubiomes/generator.h"
#include <cassert>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace Version
{

constexpr int32_t getMinYAtGivenMCVersion(int minecraftVersion)
{
    switch(minecraftVersion)
    {
        case MCVersion::MC_1_18_2:
            return -64;
        case MCVersion::MC_1_19_2:
            return -64;
        case MCVersion::MC_1_19_4:
            return -64;
        case MCVersion::MC_1_20_6:
            return -64;
        case MCVersion::MC_1_21_1:
            return -64;
        case MCVersion::MC_1_21_3:
            return -64;
        case MCVersion::MC_1_21_WD:
            return -64;
    }
    return 0;
}

auto getMinecraftVersionFromUserInput()
{
    using Version = std::pair<std::string_view, int>;

    const std::array versions = std::to_array<Version>({
        {"1.14-1.14.4",   MC_1_14_4},
        {"1.15-1.15.2",   MC_1_15_2},
        {"1.16-1.16.1",   MC_1_16_1},
        {"1.16.2-1.16.5", MC_1_16_5},
        {"1.17-1.17.1",   MC_1_17_1},
        {"1.18-1.18.2",   MC_1_18_2},
        {"1.19-1.19.2",   MC_1_19_2},
        {"1.19.3-1.19.4", MC_1_19_4},
        {"1.20-1.20.6",   MC_1_20_6},
        {"1.21-1.21.1",   MC_1_21_1},
        {"1.21.2-1.21.3", MC_1_21_3}
    });

    std::cout << "Select the minecraft version\n";

    std::size_t i = 0;
    for(const auto& version : versions)
    {
        std::string indexStr("(");
             indexStr.append(std::to_string(i++))
                     .append(")")
                     .resize(5, ' ');
        std::cout << indexStr
                  << version.first
                  << std::endl;
    }
    
    std::size_t versionIndex = 0;
    std::cin >> versionIndex;
    if(versionIndex >= versions.size())
    {
        std::cout << "Please choose proper minecraft version index" << std::endl;
        std::exit(1);
    }

    return versions[versionIndex].second;
}

}