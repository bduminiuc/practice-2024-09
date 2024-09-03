#include <cassert>
#include <iostream>

class Custom_Vector
{
private:
    size_t _size{};
    size_t _capacity{};
    int *buffer;

public:
    Custom_Vector()
        : _size(0),
          _capacity(0),
          buffer(nullptr) {}

    explicit Custom_Vector(size_t size)
        : _size(size),
          _capacity(size),
          buffer(new int[_capacity])
    {
        std::fill_n(buffer, _size, int());
    }

    Custom_Vector(const std::initializer_list<int> list)
        : Custom_Vector(list.size())
    {
        std::copy(list.begin(), list.end(), buffer);
    }

    Custom_Vector(const Custom_Vector &other)
        : Custom_Vector(other._size)
    {
        std::copy(other.buffer, other.buffer + other._size, buffer);
    }

    Custom_Vector(Custom_Vector &&other)
        : Custom_Vector()
    {
        std::swap(buffer, other.buffer);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    ~Custom_Vector() { delete[] buffer; }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    void assign(size_t count, const int &value)
    {
        clear();
        reserve(count);

        _size = count;

        std::fill_n(buffer, _size, value);
    }

    void assign(std::initializer_list<int> list)
    {
        clear();

        reserve(list.size());

        _size = list.size();

        std::copy(list.begin(), list.end(), buffer);
    }

    void clear()
    {
        _size = 0;
    }

    void push_back(const int &element)
    {
        if (_capacity == 0)
        {
            reserve(1);
        }
        if (_size == _capacity)
        {
            reserve(2 * _capacity);
        }

        buffer[_size] = element;
        _size++;
    }

    void reserve(size_t size_n)
    {
        if (size_n <= _capacity)
        {
            return;
        }

        int *point = new int[size_n];

        std::copy(buffer, buffer + _size, point);

        delete[] buffer;
        std::swap(buffer, point);
        _capacity = size_n;
    }

    int at(size_t pos)
    {
        if (_size - 1 < pos || _size == 0)
        {
            throw std::out_of_range("Error");
        }
        return buffer[pos];
    }

    const int &at(size_t pos) const
    {
        if (_size - 1 < pos || _size == 0)
        {
            throw std::out_of_range("Error");
        }
        else
        {
            return buffer[pos];
        }
    }

    int &operator[](size_t pos) { return buffer[pos]; }
    const int &operator[](size_t pos) const { return buffer[pos]; }

    Custom_Vector &operator=(const Custom_Vector &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (other._size <= _capacity)
        {
            clear();
            std::copy(other.buffer, other.buffer + other._size, buffer);
            _size = other._size;
            return *this;
        }

        int *point = new int[other._capacity];
        std::copy(other.buffer, other.buffer + other._size, point);

        delete[] buffer;
        _size = other._size;
        _capacity = other._capacity;
        std::swap(buffer, point);
        return *this;
    }

    void erase(size_t pos)
    {
        if (_size < pos || _size == 0)
        {
            throw std::out_of_range("Error");
        }

        for (size_t i = pos; i < _size; i++)
        {
            buffer[i] = buffer[i + 1];
        }
        _size--;
    }
};

int main()
{
    Custom_Vector a = {1, 4, 7, 9};
    assert(a[0] == 1);
    assert(a.at(2) == 7);
    // assert(a.at(5));//выкинет исключение
    assert(a.size() == 4);
    assert(a.capacity() == 4);
    a.push_back(5);
    assert(a.size() == 5);
    assert(a.capacity() == 8);
    assert(a[4] == 5);
    Custom_Vector b = a;
    for (size_t i = 0; i < b.size(); i++)
    {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
    b.clear();
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.erase(2);
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.clear();
    Custom_Vector c;
    c.assign(4, 9);
    for (size_t i = 0; i < c.size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.assign(2, 7);
    for (size_t i = 0; i < c.size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.assign({1, 2, 3, 4, 5, 6});
    for (size_t i = 0; i < c.size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.erase(2);
    for (size_t i = 0; i < c.size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.clear();
    assert(c.size() == 0);
    assert(c.capacity() == 6);
    return 0;
}