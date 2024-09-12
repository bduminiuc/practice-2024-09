#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>
#include "gtest/gtest.h"

template <typename T>
class Node
{
public:
    T data{};
    Node<T> *_next;
    Node<T> *_prev;

    Node() : data(int()), _next(nullptr), _prev(nullptr) {}
    Node(const T &value) : data(value), _next(nullptr), _prev(nullptr) {}

    void add(Node<T> *new_Node)
    {
        new_Node->_next = this;
        new_Node->_prev = _prev;
        _prev->_next = new_Node;
        _prev = new_Node;
    }

    void remove()
    {
        _next->_prev = _prev;
        _prev->_next = _next;
        _next = _prev = nullptr;
    }

    void swap(Node<T> &other) { std::swap(data, other.data); }

    void rev() { std::swap(_next, _prev); }
};

template <typename T>
class RawIterator
{
private:
    Node<T> *_ptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    RawIterator() : _ptr(nullptr) {}
    RawIterator(Node<T> *other) : _ptr(other) {}
    RawIterator(const RawIterator<T> &other) : _ptr(other._ptr) {}
    ~RawIterator() = default;

    reference operator*() { return _ptr->data; }
    Node<T> *ptr() { return _ptr; }

    RawIterator<T> operator++()
    {
        _ptr = _ptr->_next;
        return *this;
    }
    RawIterator<T> operator++(int)
    {
        RawIterator<T> tmp(*this);
        ++(*this);
        return tmp;
    }

    RawIterator<T> operator--()
    {
        _ptr = _ptr->_prev;
        return *this;
    }
    RawIterator<T> operator--(int)
    {
        RawIterator tmp(*this);
        --(*this);
        return tmp;
    }

    friend bool operator==(const RawIterator<T> &a, const RawIterator<T> &b)
    {
        return a._ptr == b._ptr;
    };
    friend bool operator!=(const RawIterator<T> &a, const RawIterator<T> &b)
    {
        return !(a == b);
    };
};

template <typename T>
class ConstRawIterator
{
private:
    const Node<T> *_ptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;

    ConstRawIterator() : _ptr(nullptr) {}
    ConstRawIterator(Node<T> *other) : _ptr(other) {}
    ConstRawIterator(const ConstRawIterator<T> &other) : _ptr(other._ptr) {}
    ~ConstRawIterator() = default;

    reference operator*() { return _ptr->data; }
    const Node<T> *ptr() { return _ptr; }

    ConstRawIterator<T> operator++()
    {
        _ptr = _ptr->_next;
        return *this;
    }
    ConstRawIterator<T> operator++(int)
    {
        ConstRawIterator<T> tmp(*this);
        ++(*this);
        return tmp;
    }

    ConstRawIterator<T> operator--()
    {
        _ptr = _ptr->_prev;
        return *this;
    }
    ConstRawIterator<T> operator--(int)
    {
        ConstRawIterator tmp(*this);
        --(*this);
        return tmp;
    }

    friend bool operator==(const ConstRawIterator<T> &a,
                           const ConstRawIterator<T> &b)
    {
        return a._ptr == b._ptr;
    };
    friend bool operator!=(const ConstRawIterator<T> &a,
                           const ConstRawIterator<T> &b)
    {
        return !(a == b);
    };
};

template <typename T>
class Custom_List
{
private:
    size_t _size{};
    Node<T> *_head;
    Node<T> *_tail;

public:
    typedef RawIterator<T> iterator;
    typedef ConstRawIterator<T> const_iterator;

    typedef std::reverse_iterator<iterator> riterator;
    typedef std::reverse_iterator<const_iterator> const_riterator;

    Custom_List() : _size(0), _head(new Node<T>()), _tail(new Node<T>())
    {
        _head->_next = _tail;
        _head->_prev = nullptr;
        _tail->_next = nullptr;
        _tail->_prev = _head;
    }

    Custom_List(size_t size_n) : Custom_List()
    {
        for (size_t i = 0; i < size_n; i++)
        {
            push_back(T());
        }
    }

    Custom_List(iterator a, iterator b) : Custom_List()
    {
        for (auto it = a; it != b; ++it)
        {
            push_back(*it);
        }
    }

    Custom_List(size_t count, T value) : Custom_List()
    {
        for (size_t i = 0; i < count; i++)
        {
            push_back(value);
        }
    }

    Custom_List(std::initializer_list<T> list) : Custom_List()
    {
        for (auto node : list)
        {
            push_back(node);
        }
    }

    Custom_List(const Custom_List &other) : Custom_List()
    {
        for (auto node : other)
        {
            push_back(node);
        }
    }

    Custom_List(Custom_List &&other) noexcept : Custom_List() { splice(cbegin(), other); }

    ~Custom_List()
    {
        clear();

        _head = _tail = nullptr;
    }

    Custom_List &operator=(const Custom_List &other)
    {
        if (this == &other)
        {
            return *this;
        }

        iterator it_begin = begin();
        iterator it_end = end();
        const_iterator other_begin = other.begin();
        const_iterator other_end = other.end();

        while (it_begin != it_end && other_begin != other_end)
        {
            *it_begin = *other_begin;
            ++it_begin;
            ++other_begin;
        }

        while (it_begin != it_end)
        {
            iterator tmp = it_begin;
            ++it_begin;
            erase(tmp);
        }

        while (other_begin != other_end)
        {
            push_back(*other_begin);
            ++other_begin;
        }

        return *this;
    }

    Custom_List &operator=(Custom_List &&other) noexcept
    {
        if (*this == other)
        {
            return *this;
        }

        clear();
        splice(cbegin(), other);

        return *this;
    }

    Custom_List &operator=(std::initializer_list<T> list)
    {
        clear();

        for (auto node : list)
        {
            push_back(node);
        }

        return *this;
    }

    void assign(size_t count) { assign(count, T()); }

    void assign(size_t count, const T &value)
    {
        clear();

        for (size_t i = 0; i < count; i++)
        {
            push_back(value);
        }
    }

    void assign(std::initializer_list<T> list)
    {
        clear();

        for (auto node : list)
        {
            push_back(node);
        }
    }

    void clear() noexcept
    {
        while (_size > 0)
        {
            erase(begin());
        }
    }

    void insert(const_iterator pos, T value)
    {
        Node<T> *new_Node(new Node<T>(value));

        Node<T> *tmp(const_cast<Node<T> *>(pos.ptr()));

        tmp->add(new_Node);

        _size++;
    }

    void push_back(T value) { insert(cend(), value); }
    void pop_back() { erase(std::prev(end())); }

    void push_front(T value) { insert(cbegin(), value); }
    void pop_front() { erase(begin()); }

    size_t size() const { return _size; }

    T &front() { return *begin(); }

    const T &front() const { return *begin(); }

    T &back() { return *std::prev(end()); }

    const T &back() const { return *std::prev(end()); }

    bool empty() const { return _size == 0; }

    void erase(iterator pos)
    {
        if (pos != end())
        {
            Node<T> *tmp(pos.ptr());
            tmp->remove();

            delete tmp;
            _size--;
        }
    }

    void resize(size_t size_n) { resize(size_n, T()); }

    void resize(size_t count, const T &value)
    {
        if (_size < count)
        {
            for (size_t i = _size; i < count; i++)
            {
                push_back(value);
            }
        }
        else
        {
            for (size_t i = _size; _size > count; i--)
            {
                pop_back();
            }
        }
    }

    void swap(Custom_List &other) noexcept
    {
        if (*this == other)
        {
            return;
        }

        std::swap(_head, other._head);
        std::swap(_tail, other._tail);
        std::swap(_size, other._size);
    }

    void merge(Custom_List &other)
    {
        if (this == &other)
        {
            return;
        }

        iterator it_begin(begin());
        iterator it_end(end());
        iterator other_begin(other.begin());
        iterator other_end(other.end());

        while (it_begin != it_end && other_begin != other_end)
        {
            if (*other_begin < *it_begin)
            {
                Node<T> *tmp(other_begin.ptr());
                ++other_begin;
                tmp->remove();
                --other._size;
                it_begin.ptr()->add(tmp);
                ++_size;
            }
            else
            {
                ++it_begin;
            }
        }

        splice(cend(), other);
    }

    void splice(const_iterator pos, Custom_List &other) noexcept
    {
        if (other.empty())
        {
            return;
        }

        iterator current{const_cast<Node<T> *>(pos.ptr())};
        iterator fin(other._tail->_prev);

        other.begin().ptr()->_prev = current.ptr()->_prev;
        fin.ptr()->_next = current.ptr();

        current.ptr()->_prev->_next = other.begin().ptr();
        current.ptr()->_prev = fin.ptr();

        _size += other._size;

        other._size = 0;

        delete other._head;
        delete other._tail;
    }

    void reverse()
    {
        iterator it = begin();

        while (it != end())
        {
            it.ptr()->rev();
            --it;
        }

        _head->rev();
        _tail->rev();
        std::swap(_head, _tail);
    }

    void sort() { MergeSort(begin(), end(), _size); }

    void MergeSort(iterator begin, iterator end, size_t size)
    {
        if (begin == end || std::next(begin) == end)
        {
            return;
        }

        iterator middle(begin);
        std::advance(middle, size / 2);

        MergeSort(begin, middle, size / 2);
        MergeSort(middle, end, size / 2 + size % 2);

        std::inplace_merge(begin, middle, end);
    }

    iterator begin() { return iterator(_head->_next); }
    const_iterator begin() const { return const_iterator(_head->_next); }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return iterator(_tail); }
    const_iterator end() const { return const_iterator(_tail); }
    const_iterator cend() const { return end(); }

    riterator rbegin() { return riterator(_tail->_prev); }
    const_riterator rbegin() const { return const_riterator(_tail->_prev); }
    const_riterator crbegin() const { return rbegin(); }

    riterator rend() { return riterator(_head); }
    const_riterator rend() const { return const_riterator(_head); }
    const_riterator crcend() const { return rend(); }
};

template <typename T>
bool operator==(const Custom_List<T> &lhs, const std::list<T> &rhs)
{
    EXPECT_EQ(lhs.size(), rhs.size());
    EXPECT_TRUE(std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    return true;
}

template <typename T>
bool operator==(const Custom_List<T> &lhs, const Custom_List<T> &rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

TEST(List_Test, constructorByDefault)
{
    Custom_List<int> test;

    EXPECT_TRUE(test.empty());
}

TEST(List_Test, constructorBySize)
{
    {
        Custom_List<int> test(0);
        EXPECT_TRUE(test.empty());
    }

    {
        Custom_List<int> test(1);
        EXPECT_EQ(test.size(), 1);
    }

    {
        Custom_List<int> test(100);
        EXPECT_EQ(test.size(), 100);
    }

    Custom_List<int> test1(8);
    std::list<int> test2(8);

    EXPECT_EQ(test1.size(), test2.size());
}

TEST(List_Test, constructorByInitList)
{
    {
        std::initializer_list<int> tmp;
        Custom_List<int> test(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1};
        Custom_List<int> test(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    std::initializer_list<int> tmp = {1, 2, 3, 4};
    Custom_List<int> test1(tmp);
    std::list<int> test2(tmp);

    EXPECT_TRUE(test1 == test2);
}

TEST(List_Test, moveConstructor)
{
    {
        Custom_List<int> tmp;
        Custom_List<int> test = std::move(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        Custom_List<int> tmp = {1};
        Custom_List<int> test = std::move(tmp);
        EXPECT_EQ(test.size(), 1);
    }

    {
        Custom_List<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test = std::move(tmp);
        EXPECT_EQ(test.size(), 10);
    }

    Custom_List<int> tmp1 = {1, 2, 3, 4, 5};
    Custom_List<int> test1 = std::move(tmp1);

    std::list<int> tmp2 = {1, 2, 3, 4, 5};
    std::list<int> test2 = std::move(tmp2);

    EXPECT_TRUE(test1 == test2);
}

TEST(List_Test, copyConstructor)
{
    {
        Custom_List<int> tmp;
        Custom_List<int> test(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        Custom_List<int> tmp = {1};
        Custom_List<int> test(tmp);
        EXPECT_TRUE(test == tmp);
    }

    {

        Custom_List<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test(tmp);
        EXPECT_TRUE(test == tmp);
    }

    Custom_List<int> tmp1 = {1, 2, 3, 4, 5};
    Custom_List<int> test1(tmp1);

    std::list<int> tmp2 = {1, 2, 3, 4, 5};
    std::list<int> test2(tmp2);

    EXPECT_TRUE(test1 == test2);
}

TEST(List_Test, assignByCountAndValue)
{

    {
        Custom_List<int> test;
        test.assign(0, 1);
        EXPECT_TRUE(test.empty());
    }

    {
        Custom_List<int> test;
        test.assign(1, 2);
        EXPECT_EQ(test.size(), 1);
    }

    {
        Custom_List<int> test;
        test.assign(100, 3);
        EXPECT_EQ(test.size(), 100);
    }

    Custom_List<int> test1;
    std::list<int> test2;

    test1.assign(5, 10);
    test2.assign(5, 10);

    EXPECT_TRUE(test1 == test2);

    test1.assign(3, 1);
    test2.assign(3, 1);

    EXPECT_TRUE(test1 == test2);

    test1.assign(10, 2);
    test2.assign(10, 2);

    EXPECT_TRUE(test1 == test2);
}

TEST(List_Test, assignByInitList)
{

    {
        std::initializer_list<int> tmp;
        Custom_List<int> test;
        test.assign(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1};
        Custom_List<int> test;
        test.assign(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test;
        test.assign(tmp);
        EXPECT_EQ(test.size(), tmp.size());
    }

    std::initializer_list<int> tmp1 = {1, 1, 1, 1, 1, 1, 1};
    std::initializer_list<int> tmp2 = {2, 2, 2};
    std::initializer_list<int> tmp3 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};

    Custom_List<int> test1;
    std::list<int> test2;

    test1 = tmp1;
    test2 = tmp1;

    EXPECT_TRUE(test1 == test2);

    test1 = tmp2;
    test2 = tmp2;

    EXPECT_TRUE(test1 == test2);

    test1 = tmp3;
    test2 = tmp3;

    EXPECT_TRUE(test1 == test2);
}

TEST(List_Test, operator_assign)
{
    {
        Custom_List<int> tmp;
        Custom_List<int> test = tmp;
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        Custom_List<int> tmp = {1};
        Custom_List<int> test = tmp;
        EXPECT_TRUE(test == tmp);
    }

    {
        Custom_List<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test = tmp;
        EXPECT_TRUE(tmp == test);
    }

    {
        std::initializer_list<int> tmp;
        Custom_List<int> test = tmp;
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1};
        Custom_List<int> test = tmp;
        EXPECT_EQ(test.size(), tmp.size());
    }

    {
        std::initializer_list<int> tmp = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Custom_List<int> test = tmp;
        EXPECT_EQ(test.size(), tmp.size());
    }

    Custom_List<int> tmp1 = {1, 2, 3, 4, 5};
    Custom_List<int> test1 = tmp1;

    EXPECT_TRUE(tmp1 == test1);

    std::initializer_list<int> tmp2 = {6, 7, 8, 9};
    test1 = tmp2;

    EXPECT_EQ(test1.size(), tmp2.size());

    Custom_List<int> test2 = std::move(test1);
    EXPECT_EQ(test2.size(), tmp2.size());
}

TEST(List_Test, pushBack)
{
    Custom_List<int> test = {1, 2, 3, 4, 5};

    test.push_back(6);
    EXPECT_EQ(test.back(), 6);

    test = {6, 7};

    test.push_back(8);
    EXPECT_EQ(test.back(), 8);

    test = {9, 11, 12, 13, 14, 15, 16, 17};

    test.push_back(18);
    EXPECT_EQ(test.back(), 18);
}

TEST(List_Test, pushFront)
{
    Custom_List<int> test = {1, 2, 3, 4, 5};

    test.push_front(6);
    EXPECT_EQ(test.front(), 6);

    test = {6, 7};

    test.push_front(8);
    EXPECT_EQ(test.front(), 8);

    test = {9, 11, 12, 13, 14, 15, 16, 17};

    test.push_front(18);
    EXPECT_EQ(test.front(), 18);
}

TEST(List_Test, insert)
{
    {
        Custom_List<int> test;
        test.insert(test.cbegin(), 1);

        EXPECT_FALSE(test.empty());
    }

    {
        Custom_List<int> test(8);
        test.insert(test.cend(), 1);

        EXPECT_EQ(test.back(), 1);
    }
}

TEST(List_Test, resize)
{
    {
        Custom_List<int> test;
        test.resize(10);

        EXPECT_FALSE(test.empty());
    }

    {
        Custom_List<int> test(8);
        test.resize(10);

        EXPECT_EQ(test.size(), 10);
    }

    {
        Custom_List<int> test(100);
        test.resize(10);

        EXPECT_EQ(test.size(), 10);
    }
}

TEST(List_Test, swap)
{
    {
        Custom_List<int> a, b;
        a.swap(b);

        EXPECT_TRUE(a.empty());
        EXPECT_TRUE(b.empty());
    }

    {
        Custom_List<int> a, b{1};
        a.swap(b);

        EXPECT_FALSE(a.empty());
        EXPECT_TRUE(b.empty());
    }

    {
        Custom_List<int> a, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.swap(b);

        EXPECT_FALSE(a.empty());
        EXPECT_TRUE(b.empty());
    }

    {
        Custom_List<int> a{1, 2, 3, 4}, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.swap(b);

        EXPECT_EQ(a.size(), 10);
        EXPECT_EQ(b.size(), 4);
    }
}

TEST(List_Test, merge)
{
    {
        Custom_List<int> a, b;
        a.merge(b);

        EXPECT_TRUE(a.empty());
    }

    {
        Custom_List<int> a, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.merge(b);

        EXPECT_FALSE(a.empty());
    }

    {
        Custom_List<int> a{1, 2, 3, 4}, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.merge(b);

        EXPECT_EQ(a.size(), 14);
    }
}

TEST(List_Test, splice)
{
    {
        Custom_List<int> a, b;
        a.splice(a.cbegin(), b);

        EXPECT_TRUE(a.empty());
    }

    {
        Custom_List<int> a, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.splice(a.cbegin(), b);

        EXPECT_FALSE(a.empty());
    }

    {
        Custom_List<int> a{1, 2, 3, 4}, b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        a.splice(a.cbegin(), b);

        EXPECT_EQ(a.size(), 14);
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}