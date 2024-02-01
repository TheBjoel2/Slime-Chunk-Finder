# Slime-Chunk-Finder
Multi threaded finder of slime chunk clusters. Uses [AVX instructions](https://www.youtube.com/watch?v=1IAwkEdRZZw) to maximize cpu power. Has other cool optimizations. Close to maxed out in terms of performance. Probably the fastest cpu based finder.
# Why this program among the others
Having a large number of slime chunks is ofc cool, but the real thing that matters is rates of your farm. Higher rates are only possible if you have more spawning spaces. This program doesn't look for the biggest amount of chunks, but rather the biggest amount of potential spawning spaces. We call it the "score". When the program finishes, it will tell you the score, and the amount of chunks that are in use. Higher score means better. Higher amount of slime chunks means nothing if the score is not higher.
# Usage
General purpose binaries are provided in the release page. For those, who wants maximum performance, should compile it themselves. It's recommended to use gcc because of it's more aggressive optimizations, coming handy in optimizing this program, but clang also would do it well. The compilation command is as follows.
```
g++ -std=c++20 -O3 -s -flto -fwhole-program Main.cpp -march=native
```
Note that the compilation process is quite slow, it takes several minutes.

# Farm Height
Using this program is quite straightforward except for the one thing that may confuse you - farm height. Get the Y levels of the highest and the lowest spawning platforms in your farm, substract highest from the lowest, add one to this and that's the farm height. For example, the height of a typical EOL farm is 1. If you're still confused, open the issue.

Use this program wisely. Report bugs.
