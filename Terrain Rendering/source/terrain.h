#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <GL/glew.h>
#include "shader.h"

struct terrain_data {
    unsigned char *heightmap;
    int size; // size of heightmap (should be 2^n)

    // print data
    void print() {
        printf("terrain_data: size=%d", size);
        // contents of heightmap
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%d ", heightmap[i + j * size]);
            }
            printf("\n");
        }
    }
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