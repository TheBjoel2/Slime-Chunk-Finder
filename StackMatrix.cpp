template<typename ValueType, std::size_t width, std::size_t height>
class StackMatrix
{
public:
    StackMatrix() = default;
    StackMatrix(StackMatrix&&) = delete;
    void operator=(StackMatrix&&) = delete;

    ValueType& get(const std::size_t x, const std::size_t y)
    {
        return storage[y][x];
    }

    const ValueType& get(const std::size_t x, const std::size_t y) const
    {
        return storage[y][x];
    }

private:
    ValueType storage[width][height];
};
