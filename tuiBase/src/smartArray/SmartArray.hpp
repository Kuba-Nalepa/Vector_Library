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

public:
    // 1. Konstruktor domyślny
    SmartArray() : m_data(nullptr), m_size(0), m_capacity(0)
    {
    }

    // 2. Konstruktor (unsigned size) - tworzy tablicę z zarezerwowaną pamięcią
    SmartArray(unsigned size) : m_data(new T[size]), m_size(0), m_capacity(size)
    {
    }

    // Destruktor - zwalnianie pamięci dynamicznej
    ~SmartArray()
    {
        delete[] m_data;
    }

    // 3. T at(unsigned index) - zwraca element lub rzuca wyjątek
    T at(unsigned index)
    {
        if (index >= m_size)
        {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    // 4. T operator[](unsigned index) - bez sprawdzania zakresu
    T &operator[](unsigned index)
    {
        return m_data[index];
    }

    // 5. bool isEmpty() - sprawdza czy tablica jest pusta
    bool isEmpty() const
    {
        return m_size == 0;
    }

    // 6. unsigned size() - zwraca liczbę elementów
    unsigned size() const
    {
        return m_size;
    }

    // 7. unsigned capacity() - zwraca zarezerwowaną pojemność
    unsigned capacity() const
    {
        return m_capacity;
    }

    // 8. void reserve(unsigned size) - rezerwuje pamięć dla size elementów
    void reserve(unsigned size)
    {
        if (size <= m_capacity)
            return;

        T *newData = new T[size];
        for (unsigned i = 0; i < m_size; ++i)
        {
            newData[i] = m_data[i];
        }

        delete[] m_data;
        m_data = newData;
        m_capacity = size;
    }

    // 9. void resize(unsigned size) - zmiana rozmiaru, dopełnienie wartością domyślną
    void resize(unsigned size)
    {
        if (size > m_capacity)
        {
            reserve(size);
        }
        if (size > m_size)
        {
            for (unsigned i = m_size; i < size; ++i)
            {
                m_data[i] = T(); // Wartość domyślna np. 0 dla int
            }
        }
        m_size = size;
    }

    // 10. void resize(unsigned size, T elem) - zmiana rozmiaru, dopełnienie elementem elem
    void resize(unsigned size, const T &elem)
    {
        if (size > m_capacity)
        {
            reserve(size);
        }
        if (size > m_size)
        {
            for (unsigned i = m_size; i < size; ++i)
            {
                m_data[i] = elem;
            }
        }
        m_size = size;
    }

    // 11. void clear() - usuwanie wszystkich elementów
    void clear()
    {
        m_size = 0;
    }

    // 12. void insert(unsigned index, T elem) - wstawianie z przesunięciem reszty
    void insert(unsigned index, const T &elem)
    {
        if (index > m_size)
        {
            throw std::out_of_range("Index out of range");
        }

        if (m_size == m_capacity)
        {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }

        for (unsigned i = m_size; i > index; --i)
        {
            m_data[i] = m_data[i - 1];
        }

        m_data[index] = elem;
        m_size++;
    }

    // 13. void erase(unsigned index) - usuwanie elementu spod indeksu
    void erase(unsigned index)
    {
        if (index >= m_size)
        {
            throw std::out_of_range("Index out of range");
        }

        for (unsigned i = index; i < m_size - 1; ++i)
        {
            m_data[i] = m_data[i + 1];
        }
        m_size--;
    }

    // 14. void erase(unsigned index, unsigned amount) - usuwanie wielu elementów
    void erase(unsigned index, unsigned amount)
    {
        if (index >= m_size || index + amount > m_size)
        {
            throw std::out_of_range("Index or range out of bounds");
        }

        for (unsigned i = index; i < m_size - amount; ++i)
        {
            m_data[i] = m_data[i + amount];
        }
        m_size -= amount;
    }

    // 15. void pushBack(T elem) - dodawanie elementu na koniec
    void pushBack(const T &elem)
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        m_data[m_size] = elem;
        m_size++;
    }

    // 16. void popBack() - usuwanie ostatniego elementu
    void popBack()
    {
        if (m_size > 0)
        {
            m_size--;
        }
    }
};

#endif // SMARTARRAY_H