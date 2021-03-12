# Slime-Chunk-Finder
Multi threaded tool for searching for slime chunk clusters
Code is garbage, but I was not really caring about clean code when I wrote it, sorry for this.

# Getting started
All the binaries for running it on windows you can find in bin/windows directory.

In case you want to run it on linux, make sure to install SFML (sudo apt-get install libsfml-dev)
And yes, yes I wasn't able to statically link sfml, sorry :(

# How to build on linux
-Install g++ (will work on g++ 7, 10. Did not tested other compiler versions)

-Install SFML (sudo apt-get install libsfml-dev=2.4.2+dfsg-4)

-Build by doing:

g++ -c -O3 -s -g0 "Slime Chunk Finder.cpp" -o "Slime Chunk Finder.o"

g++ -O3 -s -g0 "Slime Chunk Finder.o" -lsfml-graphics -lsfml-window -lsfml-system -o "Slime Chunk Finder"

# How to build on windows
-Install mingw 7, 8 or 10 (Did not tested other compiler versions)

-Download SFML

-Your build commands will be the same as build commands on linux

# How to use application
-Close any other applications you are running(for speed)

-Run executable

seed = seed

duration - Well... When I made this application I did not know english that well to correctly describe inputs of program. So the duration means...

Imagine a square. Square - is the area where app will search for slime clusters. Sides of this square = (2a+1), where 'a' is duration
(duration in chunks, but not in blocks!!!)

thread num = thread number
