#ifndef TEXTUREDPLANE_H
#define TEXTUREDPLANE_H

#include "graphicalobject.h"

class TexturedPlane : public GraphicalObject {
    float width, depth;
    public:
        TexturedPlane(float w, float d) : width(w), depth(d) {
            name = "texturedplane";

            std::vector<GLfloat> vertices = {
                -width / 2.0f, 0.0f, depth / 2.0f,
                width / 2.0f, 0.0f, depth / 2.0f,
                width / 2.0f, 0.0f, -depth / 2.0f,
                -width / 2.0f, 0.0f, -depth / 2.0f
            };

            GLuint* indices = new GLuint[6];
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;

            GLuint EBO, VBO;
            // create VAO, EBO, VBO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &EBO);
            glGenBuffers(1, &VBO);

            // bind VAO
            glBindVertexArray(VAO);

            // bind VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
            
            // bind EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indices, GL_STATIC_DRAW);

            // vertex position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
            glEnableVertexAttribArray(0);

            // delete the vertex and index buffers
            delete[] indices;
            vertices.clear();

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