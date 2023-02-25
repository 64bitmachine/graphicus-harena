#include "array2d.h"

template<typename T>
Array2D<T>::Array2D() {
    m_width = 0;
    m_height = 0;
    m_data = 0;
}

template <typename T>
Array2D<T>::Array2D(int width, int height, T min_height, T max_height) {
    m_width = width;
    m_height = height;
    m_data = new T[width * height];
    m_minmax_height = std::make_pair(min_height, max_height);
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
void Array2D<T>::init(T* value) {
    for (long i = 0; i < m_width * m_height; i++) {
        m_data[i] = value[i];
    }
}

template <typename T>
void Array2D<T>::Normalize() {
    T min = m_data[0];
    T max = m_data[0];
    for (long i = 0; i < m_width * m_height; i++) {
        if (m_data[i] < min) {
            min = m_data[i];
        }
        if (m_data[i] > max) {
            max = m_data[i];
        }
    }
    for (long i = 0; i < m_width * m_height; i++) {
        m_data[i] = (m_data[i] - min) / (max - min) * (m_minmax_height.second - m_minmax_height.first);
        m_data[i] += m_minmax_height.first;
    }
}

template <typename T>
T Array2D<T>::getMinHeight() const { return m_minmax_height.first; }

template <typename T>
T Array2D<T>::getMaxHeight() const { return m_minmax_height.second; }

template class Array2D<float>;