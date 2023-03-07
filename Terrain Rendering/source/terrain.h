#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <GL/glew.h>
#include "shader.h"
#include "array2d.h"
#include "math_utils.h"

class Terrain;

struct Vertex {
        Vector3f position;
        // Vector2f texCoords;

        void init(const Terrain *terrain, int x, int z);
};

class Terrain {
public:
    Terrain(int size);
    ~Terrain();
    void loadHeightMap(const char *filename);
    void render();
    void setScale(float scale);
    void useProgram();
    Shader* getShader();
    

private:

    friend struct Vertex;

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