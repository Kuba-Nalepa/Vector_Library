#ifndef SMARTARRAY_H
#define SMARTARRAY_H

#include <stdexcept>

template <typename T>
class SmartArray
{
private:
    T *m_data;
    unsigned m_size;
    unsigned m_capacity;

    void ensureCapacity(unsigned size);

public:
    SmartArray();
    SmartArray(unsigned size);

    SmartArray(const SmartArray &other);
    SmartArray &operator=(const SmartArray &other);
    SmartArray(SmartArray &&other) noexcept;
    SmartArray &operator=(SmartArray &&other) noexcept;
    ~SmartArray();

    T &at(unsigned index);
    const T &at(unsigned index) const;

    T &operator[](unsigned index);
    const T &operator[](unsigned index) const;

    bool isEmpty() const;
    unsigned size() const;
    unsigned capacity() const;

    void reserve(unsigned size);
    void resize(unsigned size);
    void resize(unsigned size, const T &elem);
    void clear();
    void insert(unsigned index, const T &elem);
    void erase(unsigned index);
    void erase(unsigned index, unsigned amount);
    void pushBack(const T &elem);
    void popBack();
};

#include "SmartArray.tpp"

#endif