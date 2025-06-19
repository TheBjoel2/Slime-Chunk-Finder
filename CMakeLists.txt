cmake_minimum_required(VERSION 3.10) #I have no idea why exactly that version
project(slime-chunk-finder)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -pedantic") #me pretending to be pedantic lmao
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -flto")

if (ASAN)
    add_compile_options(-fsanitize=address)
endif()
if (UBSAN)
    add_compile_options(-fsanitize=undefined)
endif()
if (MARCH)
    add_compile_options(-march=native)
endif()

add_subdirectory(cubiomes)

add_executable(Slime-Chunk-Finder Main.cpp)
add_executable(Slime-Chunk-Finder-static Main.cpp)

target_link_libraries(Slime-Chunk-Finder PRIVATE cubiomes_static)
target_link_libraries(Slime-Chunk-Finder-static PRIVATE cubiomes_static)

if (ASAN)
    target_link_libraries(Slime-Chunk-Finder PRIVATE asan)
    target_link_libraries(Slime-Chunk-Finder-static PRIVATE asan)
endif()
if (UBSAN)
    target_link_libraries(Slime-Chunk-Finder PRIVATE ubsan)
    target_link_libraries(Slime-Chunk-Finder-static PRIVATE ubsan)
endif()

target_link_libraries(Slime-Chunk-Finder-static PRIVATE -static)

