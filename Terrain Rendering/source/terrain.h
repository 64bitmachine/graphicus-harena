#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <GL/glew.h>
#include "shader.h"

struct terrain_data {
    unsigned char *heightmap;
    int size; // size of heightmap (should be 2^n)
};

class Terrain
{
public:
    Terrain();
    ~Terrain();
    void loadHeightMap(const char *filename);
    void render();
    unsigned char getHeight(int x, int z);
    void setScale(float scale);
    float getScaledHeight(int x, int z);
    void useProgram();
    Shader* getShader();

protected:
    terrain_data m_data;
    float m_scale;

    // render data
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    // shader data
    Shader* m_shader;
};

#endif // __TERRAIN_H__