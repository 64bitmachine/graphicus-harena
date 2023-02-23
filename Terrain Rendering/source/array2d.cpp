#include "array2d.h"

template <typename T>
Array2D<T>::Array2D() {
    m_width = 0;
    m_height = 0;
    m_data = 0;
}

template <typename T>
Array2D<T>::Array2D(int width, int height) {
    m_width = width;
    m_height = height;
    m_data = new T[width * height];
}

template <typename T>
Array2D<T>::~Array2D() {
    delete[] m_data;
}

template <typename T>
T& Array2D<T>::operator()(int x, int y) {
    return m_data[y * m_width + x];
}

template <typename T>
const T& Array2D<T>::operator()(int x, int y) const {
    return m_data[y * m_width + x];
}

template <typename T>
int Array2D<T>::width() const { return m_width; }

template <typename T>
int Array2D<T>::height() const { return m_height; }

// initialize m_data using = operator
template <typename T>
void Array2D<T>::operator=(T* value) {
    m_data = value;
}