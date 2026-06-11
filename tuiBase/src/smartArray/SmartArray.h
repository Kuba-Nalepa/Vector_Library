#ifndef SMARTARRAY_H
#define SMARTARRAY_H

template <typename T>
class SmartArray
{
private:
    T *m_data;
    unsigned m_size;
    unsigned m_capacity;

public:
    SmartArray();
    SmartArray(unsigned size);
    ~SmartArray();

    T at(unsigned index);
    T operator[](unsigned index);
    bool isEmpty();
    unsigned size();
    unsigned capacity();

    void reserve(unsigned size);
    void resize(unsigned size);
    void resize(unsigned size, T elem);
    void clear();

    void insert(unsigned index, T elem);
    void erase(unsigned index);
    void erase(unsigned index, unsigned amount);
    void pushBack(T elem);
    void popBack();
};

#endif
