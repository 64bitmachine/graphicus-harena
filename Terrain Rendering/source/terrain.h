#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <GL/glew.h>
#include "shader.h"
#include "array2d.h"

class Terrain
{
public:
    Terrain(int size);
    ~Terrain();
    void loadHeightMap(const char *filename);
    void render();
    void setScale(float scale);
    void useProgram();
    Shader* getShader();
    

protected:
    Array2D<float> *m_heightmap;
    int m_size; // size of heightmap (x, z are the same)
    float m_scale;

    // render data
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    // shader data
    Shader* m_shader;
};

#endif // __TERRAIN_H__