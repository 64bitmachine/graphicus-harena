#ifndef __FRACTALTERRAIN_H__
#define __FRACTALTERRAIN_H__

#include <cstdlib>
#include "math_utils.h"

unsigned char* terrainFromFaultFormation(int size, int iterations) {

    // create terrain
    float* terrain = new float[size * size];
    // initialize terrain
    for (long i = 0; i < size * size; i++) terrain[i] = 0.0f;
    

    for(int iter = 0; iter < iterations; iter++) {

        // select random two points
        Vector3f p1(rand() % size, 0, rand() % size);
        Vector3f p2(0);
        do {
            p2.x = rand() % size;
            p2.z = rand() % size;
        } while (p1 == p2);

        p1.Print();
        p2.Print();
        std::cout << std::endl;

        float random_height = (1.0 - (float)iter/(float)iterations) * 255;
        // printf("random_height: %d", random_height);

        // select random displacement
        for(int x = 0; x < size; x++) {
            for(int z = 0; z < size; z++) {
                Vector3f p3(x, 0, z);

                if (((p3-p1).Cross(p2-p1)).y > 0) {
                    terrain[x * size + z] += random_height;
                }
            }
        }
    }

    // convert to unsigned char
    unsigned char* terrain_uchar = new unsigned char[size * size];
    float max = 0.0f;
    for (long i = 0; i < size * size; i++) {
        if (terrain[i] > max) max = terrain[i];
    }
    for (long i = 0; i < size * size; i++) {
        terrain_uchar[i] = (unsigned char)((terrain[i] / max) * 255);
    }

    delete[] terrain;

    return terrain_uchar;
}

#endif // __FRACTALTERRAIN_H__