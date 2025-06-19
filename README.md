# Slime-Chunk-Finder
Multi threaded finder of slime chunk clusters. Uses [AVX instructions](https://www.youtube.com/watch?v=1IAwkEdRZZw) to maximize cpu power. Has other cool optimizations. Supports minecraft 1.14+
# Why this program among the others
Having a large number of slime chunks is ofc cool, but the real thing that matters is rates of your farm. Higher rates are only possible if you have more spawning spaces. Well, there's actually more about it than just spawning spaces, but for now, let's not go into details. The program performs a little bit more complicated evaluations than just counting the number of chunks. And then, when it finishes, it outputs the number. We (by "we" I mean "me") call it the "score". Higher score means better. Higher number of slime chunks means nothing if the score is not higher.
# Usage
General purpose binaries are provided in the release page. For those, who wants maximum performance, should compile it themselves. At the moment, only ``gcc`` is able to compile it (it's quite easy to make it work with clang though). The sad thing is that you need [cmake](https://cmake.org/) 😭. The build process is as follows:
```
git clone --recursive https://github.com/TheBjoel2/Slime-Chunk-Finder.git
cd Slime-Chunk-Finder
mkdir build && cd build
cmake "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" "-DCMAKE_BUILD_TYPE=Release" ..
make
```
Note that the compilation process is quite slow (takes a couple minutes to finish) bc I abuse templates too much 😜.

# Farm Height
Using this program is quite straightforward except for the one thing that may confuse you - farm height. Get the Y levels of the highest and the lowest spawning platforms in your farm, substract the Y of the lowest platform from the Y of the highest platform, add one to this and that's the farm height. For example, the height of a typical Portal Spam farm is 1. If you're still confused, open the issue.

Use this program wisely. Report bugs.
