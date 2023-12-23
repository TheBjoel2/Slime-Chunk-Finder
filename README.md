# Slime-Chunk-Finder
Multi threaded finder of slime chunk clusters. Uses [AVX instructions](https://www.youtube.com/watch?v=1IAwkEdRZZw) to maximize cpu power. Has other cool optimizations. Close to maxed out in terms of performance. Probably the fastest finder that works on cpu.
# Usage
Right now binaries are not provided, so you would have to compile it yourself. Think of it as an opportunity to optimize build for your particular machine. It's recommended to use gcc because of it's more aggressive optimizations, coming handy in optimizing this program, but clang also would do it well. The compilation command is as follows.
```
g++ -std=c++20 -O3 -s -flto -fwhole-program Main.cpp -march=native
```
Launch the program -> enter required data, following the instructions ofc -> enjoy the result. Use wisely. Report bugs.