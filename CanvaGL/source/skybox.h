#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "fileutils.h"

class Cuboid;

class Skybox : public Cuboid {
    public:
        Skybox(): Cuboid(glm::vec3(0.0f), glm::vec3(200.0f, 200.0f, 200.0f), 
    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(1.0, 0.0f, 0.0f)), false) {
            name = "skybox";
            std::cout << "Skybox created" << std::endl;
        }

        void nextFrame() {
            glBindVertexArray(VAO);
            if(texture != -1) {
                glBindTexture(GL_TEXTURE_2D, texture);
                shader->setInt("cubeMap", 0);
            }
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

            if (texture != -1) glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
};

#endif