#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <GL/glew.h>
#include "shader.h"

struct terrain_data {
    // unsigned char *heightmap;
    float* heightmap;
    int size; // size of heightmap (should be 2^n)

    // print data
    void print() {
        printf("terrain_data: size = %d \n", size);
        // contents of heightmap
        // for (int i = 0; i < size; i++) {
        //     for (int j = 0; j < size; j++) {
        //         printf("%f ", heightmap[i * size + j]);
        //     }
        //     printf("\n");
        // }

        // histogram of heightmap
        int hist[10];
        for (int i = 0; i < 10; i++) hist[i] = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                hist[(int)(heightmap[i * size + j] / 30)]++;
            }
        }
        for (int i = 0; i < 10; i++) {
            printf("%d, %d\n", i, hist[i]);
        }
        // flush
        printf("\n");
    }
};

class Terrain
{
public:
    Terrain();
    ~Terrain();
    void loadHeightMap(const char *filename);
    void render();
    float getHeight(int x, int z);
    void setScale(float scale);
    float getScaledHeight(int x, int z);
    void useProgram();
    Shader* getShader();
    

protected:
    terrain_data m_data;
    float m_scale, min_height, max_height;

    // render data
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    // shader data
    Shader* m_shader;
};

#endif // __TERRAIN_H__