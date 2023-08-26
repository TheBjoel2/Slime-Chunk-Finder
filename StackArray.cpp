#ifndef STACK_ARRAY_CPP
#define STACK_ARRAY_CPP

#include <cstdint>

template<typename ValueType, std::size_t size>
class StackArray
{
public:
    StackArray() = default;
    StackArray(StackArray&&) = delete;
    void operator=(StackArray&&) = delete;

    template<typename... Args>
    void emplaceBack(Args&&... args)
    {
        values[index++] = ValueType(args...);
    }

    void popBack()
    {
        index--;
    }

    ValueType& operator[](std::size_t i)
    {
        return values[i];
    }

private:
    ValueType values[size];
    std::size_t index = 0;
};

#endif // STACK_ARRAY_CPP
