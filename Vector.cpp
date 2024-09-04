#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <algorithm>

class Custom_Vector
{
private:
    size_t _size{};
    size_t _capacity{};
    int *buffer;

public:
    typedef int *iterator;
    typedef const int *const_iterator;

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

    Custom_Vector(std::initializer_list<int> list)
        : _size(list.size()),
          _capacity(list.size()),
          buffer(new int[_capacity])
    {
        std::copy(list.begin(), list.end(), buffer);
    }

    Custom_Vector(const Custom_Vector &other)
        : _size(other._size),
          _capacity(other._size),
          buffer(new int[_capacity])
    {
        std::copy(other.buffer, other.buffer + other._size, buffer);
    }

    Custom_Vector(Custom_Vector &&other)
        : _size(other._size),
          _capacity(other._capacity),
          buffer(nullptr)
    {
        std::swap(buffer, other.buffer);
        other._size = other._capacity = 0;
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
        reserve(count);

        _size = count;

        std::fill_n(buffer, _size, value);
    }

    void assign(std::initializer_list<int> list)
    {
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

        std::copy(begin(), end(), point);

        delete[] buffer;
        buffer = point;
        _capacity = _capacity + size_n;
    }

    int at(size_t pos)
    {
        if (_size <= pos)
        {
            throw std::out_of_range("Error");
        }

        return buffer[pos];
    }

    const int &at(size_t pos) const
    {
        if (_size <= pos)
        {
            throw std::out_of_range("Error");
        }

        return buffer[pos];
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
            std::copy(other.begin(), other.end(), buffer);
            _size = other._size;
            return *this;
        }

        int *point = new int[other._capacity];
        std::copy(other.begin(), other.end(), point);

        delete[] buffer;
        _size = other._size;
        _capacity = other._capacity;
        buffer = point;
        return *this;
    }

    Custom_Vector &operator=(Custom_Vector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        _size = other._size;
        _capacity = other._capacity;
        delete[] buffer;

        std::swap(buffer, other.buffer);
        other._size = other._capacity = 0;
        return *this;
    }

    Custom_Vector &operator=(std::initializer_list<int> list)
    {
        assign(list);
        return *this;
    }

    void erase(size_t pos)
    {
        if (_size <= pos)
        {
            throw std::out_of_range("Error");
        }

        for (size_t i = pos; i < _size; i++)
        {
            buffer[i] = buffer[i + 1];
        }
        _size--;
    }

    iterator begin() { return buffer; }
    const_iterator begin() const { return buffer; }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return buffer + _size; }
    const_iterator end() const { return buffer + _size; }
    const_iterator cend() const { return end(); }
};

bool operator==(const Custom_Vector &lhs, const Custom_Vector &rhs)
{
    if (lhs.size() != rhs.size() || lhs.capacity() != rhs.capacity())
    {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

bool operator==(const Custom_Vector &lhs, const std::vector<int> &rhs)
{
    EXPECT_NE(lhs.size(), 0);
    EXPECT_EQ(lhs.size(), rhs.size());
    EXPECT_EQ(lhs.capacity(), rhs.capacity());
    EXPECT_TRUE(std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    return true;
}

bool equivalent(const Custom_Vector &lhs, const std::vector<int> &rhs)
{
    EXPECT_NE(lhs.size(), 0);
    EXPECT_EQ(lhs.size(), rhs.size());
    EXPECT_LE(lhs.size(), lhs.capacity());
    EXPECT_TRUE(std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    return true;
}

TEST(Vector_Test, constructorByDefault)
{
    Custom_Vector test;

    EXPECT_EQ(test.size(), 0);
    EXPECT_EQ(test.capacity(), 0);
}

TEST(Vector_Test, constructorBySize)
{
    Custom_Vector test1(8);
    std::vector<int> test2(8);

    EXPECT_NE(test1.size(), 0);
    EXPECT_EQ(test1.size(), test2.size());
    EXPECT_EQ(test1.capacity(), test2.capacity());
}

TEST(Vector_Test, constructorByInitList)
{
    std::initializer_list<int> tmp = {1, 2, 3, 4};
    Custom_Vector test1(tmp);
    std::vector<int> test2(tmp);

    EXPECT_TRUE(test1 == test2);
}

TEST(Vector_Test, copyConstructor)
{
    Custom_Vector tmp1 = {1, 2, 3, 4, 5};
    Custom_Vector test1(tmp1);

    EXPECT_TRUE(test1 == tmp1);

    std::vector<int> tmp2 = {1, 2, 3, 4, 5};
    std::vector<int> test2(tmp2);

    EXPECT_TRUE(test1 == test2);
}

TEST(Vector_Test, moveConstructor)
{
    Custom_Vector tmp1 = {1, 2, 3, 4, 5};
    Custom_Vector test1 = std::move(tmp1);

    std::vector<int> tmp2 = {1, 2, 3, 4, 5};
    std::vector<int> test2 = std::move(tmp2);

    EXPECT_TRUE(test1 == test2);
}

TEST(Vector_Test, at)
{
    Custom_Vector test1;

    EXPECT_THROW(test1.at(0), std::out_of_range);
    EXPECT_THROW(test1.at(-1), std::out_of_range);
    EXPECT_THROW(test1.at(1), std::out_of_range);

    const Custom_Vector test2 = {1, 2, 3, 4, 5};
    EXPECT_THROW(test2.at(-1), std::out_of_range);
    EXPECT_THROW(test2.at(5), std::out_of_range);
    EXPECT_EQ(test2.at(1), 2);
}

TEST(Vector_Test, assignByCountAndValue)
{
    Custom_Vector test1;
    std::vector<int> test2;

    test1.assign(5, 10);
    test2.assign(5, 10);

    EXPECT_TRUE(equivalent(test1, test2));

    test1.assign(3, 1);
    test2.assign(3, 1);

    EXPECT_TRUE(equivalent(test1, test2));

    test1.assign(10, 2);
    test2.assign(10, 2);

    EXPECT_TRUE(equivalent(test1, test2));
}

TEST(Vector_Test, assignByInitList)
{
    std::initializer_list<int> tmp1 = {1, 1, 1, 1, 1, 1, 1};
    std::initializer_list<int> tmp2 = {2, 2, 2};
    std::initializer_list<int> tmp3 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};

    Custom_Vector test1;
    std::vector<int> test2;

    test1 = tmp1;
    test2 = tmp1;

    EXPECT_TRUE(equivalent(test1, test2));

    test1 = tmp2;
    test2 = tmp2;

    EXPECT_TRUE(equivalent(test1, test2));

    test1 = tmp3;
    test2 = tmp3;

    EXPECT_TRUE(equivalent(test1, test2));
}

TEST(Vector_Test, operator_assign)
{
    Custom_Vector tmp1 = {1, 2, 3, 4, 5};
    Custom_Vector test1 = tmp1;

    EXPECT_NE(test1.size(), 0);
    EXPECT_TRUE(tmp1 == test1);

    std::initializer_list<int> tmp2 = {6, 7, 8, 9};
    test1 = tmp2;

    EXPECT_TRUE(equivalent(test1, tmp2));

    Custom_Vector test2 = std::move(test1);
    EXPECT_TRUE(equivalent(test2, tmp2));
}

TEST(Vector_Test, pushBack)
{
    Custom_Vector test = {1, 2, 3, 4, 5};

    EXPECT_NE(test.size(), 0);

    test.push_back(6);
    EXPECT_EQ(test[test.size() - 1], 6);
    EXPECT_LE(test.size(), test.capacity());

    test = {6, 7};

    test.push_back(8);
    EXPECT_EQ(test[test.size() - 1], 8);
    EXPECT_LE(test.size(), test.capacity());

    test = {9, 11, 12, 13, 14, 15, 16, 17};

    test.push_back(18);
    EXPECT_EQ(test[test.size() - 1], 18);
    EXPECT_LE(test.size(), test.capacity());
}
TEST(Vector_Test, reserve)
{
    Custom_Vector test;

    test.reserve(5);

    EXPECT_EQ(test.capacity(), 5);
    EXPECT_LE(test.size(), test.capacity());

    test.reserve(2);

    EXPECT_NE(test.capacity(), 2);
    EXPECT_LE(test.size(), test.capacity());

    test.reserve(10);

    EXPECT_GE(test.capacity(), 5);
    EXPECT_LE(test.size(), test.capacity());
}

TEST(Vector_Test, erase)
{
    Custom_Vector test;

    EXPECT_EQ(test.size(), 0);
    EXPECT_THROW(test.erase(-1), std::out_of_range);
    EXPECT_THROW(test.erase(10), std::out_of_range);

    test = {1, 2, 3, 4, 5};

    EXPECT_NE(test.size(), 0);
    test.erase(3);
    EXPECT_EQ(test[3], 5);
    EXPECT_THROW(test.erase(4), std::out_of_range);
}

TEST(Vector_Test, input)
{
    Custom_Vector tmp1;
    Custom_Vector test1(tmp1);
    EXPECT_EQ(test1.size(), 0);

    std::initializer_list<int> tmp2;

    Custom_Vector test2(tmp2);
    EXPECT_EQ(test1.size(), 0);

    test1 = tmp2;
    EXPECT_EQ(test1.size(), 0);

    test1.assign(0, 0);
    EXPECT_EQ(test1.size(), 0);

    test1.assign(tmp2);
    EXPECT_EQ(test1.size(), 0);

    test1 = tmp1;
    EXPECT_EQ(test1.size(), 0);

    Custom_Vector tmp3 = {1,2,3,4,5};
    Custom_Vector test3(tmp3);
    EXPECT_NE(test3.size(), 0);

    test3.clear();
    std::initializer_list<int> tmp4 = {6,7,8,9};

    Custom_Vector test4(tmp3);
    EXPECT_NE(test4.size(), 0);

    test3 = tmp4;
    EXPECT_NE(test3.size(), 0);

    test3.clear();
    test3.assign(3, 2);
    EXPECT_NE(test3.size(), 0);

    test3.clear();
    test3.assign(tmp4);
    EXPECT_NE(test3.size(), 0);

    test3.clear();
    test3 = tmp4;
    EXPECT_NE(test3.size(), 0);

    Custom_Vector test5 = std::move(tmp1);
    EXPECT_EQ(test1.size(), 0);

    Custom_Vector test6 = std::move(tmp3);
    EXPECT_NE(test6.size(), 0);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}