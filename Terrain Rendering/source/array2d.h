#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

template <typename T>
class Array2D {
public:

    Array2D();

    Array2D(int width, int height);

    ~Array2D();

    T& operator()(int x, int y);

    const T& operator()(int x, int y) const ;

    int width() const;
    int height() const;

    // initialize m_data using = operator
    void operator=(T* value) ;

private:
    T* m_data;
    int m_width;
    int m_height;
};

#endif // __ARRAY2D_H__