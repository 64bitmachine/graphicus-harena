#ifndef __FRACTALTERRAIN_H__
#define __FRACTALTERRAIN_H__

#include <cstdlib>
#include "math_utils.h"
#include <limits>


float FIR_filter_pt(int x, int z, float filter_param, float prevVal, float currVal) {
    return filter_param * prevVal + (1 - filter_param) * currVal;
}

float* terrainFromFaultFormation(int terrain_size, int iterations, float terrain_max_height, float terrain_min_height, float filter_param) {

    float delta_height = terrain_max_height - terrain_min_height;

    // create terrain
    float* terrain = new float[terrain_size * terrain_size];
    // initialize terrain
    for (long i = 0; i < terrain_size * terrain_size; i++) terrain[i] = 0.0f;
    float maxHeight = 0.0f;
    float minHeight = std::numeric_limits<float>::max();

    for(int iter = 0; iter < iterations; iter++) {

        // select random two points
        Vector3f p1(rand() % terrain_size, 0, rand() % terrain_size);
        Vector3f p2(0);
        do {
            p2.x = rand() % terrain_size;
            p2.z = rand() % terrain_size;
        } while (p1 == p2);

        // p1.Print();
        // p2.Print();
        // std::cout << std::endl;

        float random_height = terrain_max_height - ((float)iter/(float)iterations) * delta_height;
        // printf("random_height: %f\n", random_height);
        // printf("random_height: %d", random_height);

        // select random displacement
        for(int x = 0; x < terrain_size; x++) {
            for(int z = 0; z < terrain_size; z++) {
                Vector3f p3(x, 0, z);
                if (((p3-p1).Cross(p2-p1)).y >= 0) {
                    terrain[x * terrain_size + z] += random_height;
                }
            }
        }
    }

    // apply FIR filter
    // left to right sweep
    for(int z = 0; z < terrain_size; z++) {
        float prevVal = terrain[z * terrain_size];
        for(int x = 1; x < terrain_size; x++) {
            prevVal = FIR_filter_pt(x, z, filter_param, prevVal, terrain[x + z * terrain_size]);
            terrain[x + z * terrain_size] = prevVal;
        }
    }

    // right to left sweep
    for(int z = 0; z < terrain_size; z++) {
        float prevVal = terrain[z * terrain_size + terrain_size - 1];
        for(int x = terrain_size - 2; x >= 0; x--) {
            prevVal = FIR_filter_pt(x, z, filter_param, prevVal, terrain[x + z * terrain_size]);
            terrain[x + z * terrain_size] = prevVal;
        }
    }

    // top to bottom sweep
    for(int x = 0; x < terrain_size; x++) {
        float prevVal = terrain[x];
        for(int z = 1; z < terrain_size; z++) {
            prevVal = FIR_filter_pt(x, z, filter_param, prevVal, terrain[x + z * terrain_size]);
            terrain[x + z * terrain_size] = prevVal;
        }
    }

    // bottom to top sweep
    for(int x = 0; x < terrain_size; x++) {
        float prevVal = terrain[(terrain_size - 1) * terrain_size + x];
        for(int z = terrain_size - 2; z >= 0; z--) {
            prevVal = FIR_filter_pt(x, z, filter_param, prevVal, terrain[x + z * terrain_size]);
            terrain[x + z * terrain_size] = prevVal;
        }
    }

    // find min and max height
    for (long i = 0; i < terrain_size * terrain_size; i++) {
        if (terrain[i] > maxHeight) maxHeight = terrain[i];
        if (terrain[i] < minHeight) minHeight = terrain[i];
    }

    // convert to unsigned char
    // unsigned char* terrain_uchar = new unsigned char[terrain_size * terrain_size];
    float hr;
    for (long i = 0; i < terrain_size * terrain_size; i++) {
        hr = (terrain[i] - minHeight) / (maxHeight - minHeight);
        terrain[i] = terrain_min_height + hr * delta_height;
        if (hr > 1.0f || hr < 0.0f) {
            printf("hr: %f\n", hr);
        }
        // terrain_uchar[i] = (unsigned char) terrain[i];
    }
    printf("\n");

    // delete[] terrain;

    return terrain;
}

#endif // __FRACTALTERRAIN_H__