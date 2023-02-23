#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <utility>

template <typename T>

class Array2D {
public:

    Array2D<T>();

    Array2D<T>(int width, int height, T min_height, T max_height);

    ~Array2D<T>();

    T& operator()(int x, int y);

    const T& operator()(int x, int y) const ;

    int width() const;
    int height() const;

    // initialize m_data using = operator
    void init(T* value) ;

    void Normalize();

    T getMinHeight() const ;
    T getMaxHeight() const ;

private:
    T* m_data;
    int m_width;
    int m_height;
    std::pair<T, T> m_minmax_height;
};

#endif // __ARRAY2D_H__