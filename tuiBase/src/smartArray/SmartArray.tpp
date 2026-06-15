#ifndef SMARTARRAY_TPP
#define SMARTARRAY_TPP

#include <stdexcept>
#include <utility>

template <typename T>
void SmartArray<T>::ensureCapacity(unsigned size)
{
    if (size <= m_capacity)
        return;

    T *newData = new T[size];
    for (unsigned i = 0; i < m_size; i++)
    {
        newData[i] = m_data[i];
    }
    delete[] m_data;
    m_data = newData;
    m_capacity = size;
}

template <typename T>
SmartArray<T>::SmartArray()
    : m_data(nullptr), m_size(0), m_capacity(0)
{
}

template <typename T>
SmartArray<T>::SmartArray(unsigned size)
    : m_data(new T[size]), m_size(0), m_capacity(size)
{
}

template <typename T>
SmartArray<T>::SmartArray(const SmartArray &other)
    : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity)
{
    if (m_capacity > 0)
    {
        m_data = new T[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            m_data[i] = other.m_data[i];
        }
    }
}

template <typename T>
SmartArray<T> &SmartArray<T>::operator=(const SmartArray &other)
{
    if (this != &other)
    {
        delete[] m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        if (m_capacity > 0)
        {
            m_data = new T[m_capacity];
            for (unsigned i = 0; i < m_size; i++)
            {
                m_data[i] = other.m_data[i];
            }
        }
        else
        {
            m_data = nullptr;
        }
    }
    return *this;
}

template <typename T>
SmartArray<T>::SmartArray(SmartArray &&other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename T>
SmartArray<T> &SmartArray<T>::operator=(SmartArray &&other) noexcept
{
    if (this != &other)
    {
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template <typename T>
SmartArray<T>::~SmartArray()
{
    delete[] m_data;
}

template <typename T>
T &SmartArray<T>::at(unsigned index)
{
    if (index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template <typename T>
const T &SmartArray<T>::at(unsigned index) const
{
    if (index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template <typename T>
T &SmartArray<T>::operator[](unsigned index)
{
    return m_data[index];
}

template <typename T>
const T &SmartArray<T>::operator[](unsigned index) const
{
    return m_data[index];
}

template <typename T>
bool SmartArray<T>::isEmpty() const
{
    return m_size == 0;
}

template <typename T>
unsigned SmartArray<T>::size() const
{
    return m_size;
}

template <typename T>
unsigned SmartArray<T>::capacity() const
{
    return m_capacity;
}

template <typename T>
void SmartArray<T>::reserve(unsigned size)
{
    if (size <= m_capacity)
        return;

    ensureCapacity(size);
}

template <typename T>
void SmartArray<T>::resize(unsigned size)
{
    if (size > m_capacity)
    {
        reserve(size);
    }
    if (size > m_size)
    {
        for (unsigned i = m_size; i < size; i++)
        {
            m_data[i] = T();
        }
    }
    m_size = size;
}

template <typename T>
void SmartArray<T>::resize(unsigned size, const T &elem)
{
    if (size > m_capacity)
    {
        reserve(size);
    }
    if (size > m_size)
    {
        for (unsigned i = m_size; i < size; i++)
        {
            m_data[i] = elem;
        }
    }
    m_size = size;
}

template <typename T>
void SmartArray<T>::clear()
{
    m_size = 0;
}

template <typename T>
void SmartArray<T>::insert(unsigned index, const T &elem)
{
    if (index > m_size)
    {
        throw std::out_of_range("Index out of range");
    }

    if (m_size == m_capacity)
    {
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }

    for (unsigned i = m_size; i > index; i--)
    {
        m_data[i] = m_data[i - 1];
    }

    m_data[index] = elem;
    m_size++;
}

template <typename T>
void SmartArray<T>::erase(unsigned index)
{
    if (index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }

    for (unsigned i = index; i < m_size - 1; i++)
    {
        m_data[i] = m_data[i + 1];
    }
    m_size--;
}

template <typename T>
void SmartArray<T>::erase(unsigned index, unsigned amount)
{
    if (index >= m_size || index + amount > m_size)
    {
        throw std::out_of_range("Index or range out of bounds");
    }

    for (unsigned i = index; i < m_size - amount; i++)
    {
        m_data[i] = m_data[i + amount];
    }
    m_size -= amount;
}

template <typename T>
void SmartArray<T>::pushBack(const T &elem)
{
    if (m_size == m_capacity)
    {
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
    m_data[m_size] = elem;
    m_size++;
}

template <typename T>
void SmartArray<T>::popBack()
{
    if (m_size > 0)
    {
        m_size--;
    }
}

#endif