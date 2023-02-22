#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

template <typename T>
class Array2D {
public:
    Array2D(int width, int height) {
        m_width = width;
        m_height = height;
        m_data = new T[width * height];
    }

    ~Array2D() {
        delete[] m_data;
    }

    T& operator()(int x, int y) {
        return m_data[y * m_width + x];
    }

    const T& operator()(int x, int y) const {
        return m_data[y * m_width + x];
    }

    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    T* m_data;
    int m_width;
    int m_height;
};

#endif // __ARRAY2D_H__