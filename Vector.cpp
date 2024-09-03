#include <cassert>
#include <iostream>

class Custom_Vector
{
private:
    size_t size;
    size_t capacity;
    int *buffer;

public:
    Custom_Vector()
        : size(0), capacity(0), buffer(nullptr) {} // конструктор по умолчанию
    Custom_Vector(int size)
        : size(0),
          capacity(size),
          buffer(new int[capacity]) {} // конструктор с заданным размером
    Custom_Vector(const std::initializer_list<int> list)
        : size(list.size()), capacity(list.size()), buffer(new int[capacity])
    {
        for (size_t i = 0; i < size; i++)
        {
            buffer[i] = *(list.begin() + i);
        }
    } // Конструктор со списком инициализации
    Custom_Vector(const Custom_Vector &other)
        : size(other.size),
          capacity(other.capacity),
          buffer(new int[capacity])
    {
        for (size_t i = 0; i < size; i++)
        {
            buffer[i] = other.buffer[i];
        }
    } // Конструктор копирования
    Custom_Vector(Custom_Vector &&other)
        : size(other.size),
          capacity(other.capacity),
          buffer(new int[capacity])
    {
        buffer = other.buffer;
        other.buffer = nullptr;
        other.size = other.capacity = 0;
    } // Конструктор перемещения
    ~Custom_Vector() { delete[] buffer; } // Деструктор
    size_t get_size()
    {
        return size;
    } // Получение количества заполненных ячеек
    size_t get_capacity()
    {
        return capacity;
    } // Получение количества всех ячеек
    void assign(size_t count, const int &value)
    {
        if (count > capacity)
        {
            reserve(count);
        }
        if (count > size)
        {
            size = count;
        }
        for (size_t i = 0; i < count; i++)
        {
            buffer[i] = value;
        }
    }
    void assign(std::initializer_list<int> list)
    {
        if (list.size() > capacity)
        {
            reserve(list.size());
        }
        if (list.size() > size)
        {
            size = list.size();
        }
        for (size_t i = 0; i < list.size(); i++)
        {
            buffer[i] = *(list.begin() + i);
        }
    }
    void clear()
    {
        std::destroy(buffer, buffer + size);
        size = 0;
    } // обнуление всех ячеек в массиве
    void push_back(const int &element)
    {
        if (capacity == 0)
        {
            reserve(8);
        }
        if (size == capacity)
        {
            reserve(2 * capacity);
        }
        buffer[size] = element;
        size++;
    } // вставка элемента в конец массива
    void reserve(size_t size_n)
    {
        if (size_n <= capacity)
        {
            return;
        }
        int *point = new int[size_n];
        for (size_t i = 0; i < size; i++)
        {
            point[i] = buffer[i];
        }
        delete[] buffer;
        buffer = point;
        capacity = size_n;
    } // Выделение памяти, если закончилось место
    int at(size_t pos)
    {
        if (size < pos)
        {
            throw std::out_of_range("Error");
        }
        return buffer[pos];
    } // Возвращает указанный элемент с проверкой на границы
    int &operator[](size_t pos)
    {
        return buffer[pos];
    } // Возвращает указанный элемент
    Custom_Vector &operator=(const Custom_Vector &other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (other.size <= capacity)
        {
            clear();
            for (size_t i = 0; i < other.size; i++)
            {
                buffer[i] = other.buffer[i];
            }
            size = other.size;
            return *this;
        }
        int *point = new int[other.capacity];
        for (size_t i = 0; i < other.size; i++)
        {
            point[i] = other.buffer[i];
        }
        delete[] buffer;
        size = other.size;
        capacity = other.capacity;
        buffer = point;
        return *this;
    } // Опреатор присваивания
    void erase(size_t pos)
    {
        if (size < pos)
        {
            throw std::out_of_range("Error");
        }
        for (size_t i = pos; i < size; i++)
        {
            buffer[i] = buffer[i + 1];
        }
        size--;
    } // Удалени�� элемента в массиве
};

int main()
{
    Custom_Vector a = {1, 4, 7, 9};
    assert(a[0] == 1);
    assert(a.at(2) == 7);
    // assert(a.at(5));//выкинет исключение
    assert(a.get_size() == 4);
    assert(a.get_capacity() == 4);
    a.push_back(5);
    assert(a.get_size() == 5);
    assert(a.get_capacity() == 8);
    assert(a[4] == 5);
    Custom_Vector b = a;
    for (size_t i = 0; i < b.get_size(); i++)
    {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
    b.clear();
    for (size_t i = 0; i < a.get_size(); i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.erase(2);
    for (size_t i = 0; i < a.get_size(); i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.clear();
    Custom_Vector c;
    c.assign(4, 9);
    for (size_t i = 0; i < c.get_size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.assign(2, 7);
    for (size_t i = 0; i < c.get_size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.assign({1, 2, 3, 4, 5, 6});
    for (size_t i = 0; i < c.get_size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.erase(2);
    for (size_t i = 0; i < c.get_size(); i++)
    {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
    c.clear();
    assert(c.get_size() == 0);
    assert(c.get_capacity() == 6);
    return 0;
}