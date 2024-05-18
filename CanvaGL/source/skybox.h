#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "graphicalobject.h"
#include "fileutils.h"

class Skybox : public GraphicalObject {
    float width, depth;
    public:
        Skybox() {
            name = "skybox";
            
            int textureWidths[6], textureHeights[6], channels[6];
            GLubyte* imageData[6];
            std::cout << "Loading skybox Images..." << std::endl;
            char const* folder = "/Users/rathod_ias/Downloads/field-skyboxes/FishPond";
            std::vector<std::string> files = getFilenames(folder);

            for (int i = 0; i < 6; i++) {
                std::cout << "Loading " << files[i] << std::endl;
                imageData[i] = readImage(files[i].c_str(), textureWidths[i], textureHeights[i], channels[i]);
                if (imageData[i] == NULL) {
                    std::cout << "Failed to load image" << std::endl;
                }
                else {
                    std::cout << "Loaded image" << std::endl;
                }
            }

            assert(glGetError()== GL_NO_ERROR);
        }

        void nextFrame() {
            glBindVertexArray(VAO);
            if(texture != -1) {
                glBindTexture(GL_TEXTURE_2D, texture);
                shader->setInt("textureMap", 0);
            }
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            if (texture != -1) glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
};

#endif