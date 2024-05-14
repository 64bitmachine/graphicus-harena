#ifndef GRID_H
#define GRID_H

#include "graphicalobject.h"

class Grid : public GraphicalObject {
    GLuint width, depth;
    public:

    Grid(float w, float d) : width(w), depth(d) {
        name = "Grid";

        std::vector<GLfloat> vertices;

        GLfloat spacingX = 1.0f, spacingZ = 1.0f;

        for (int i = 0; i < depth + 1; i++) {
            for(int j = 0; j < width + 1; j++) {
                GLfloat currZ = -d/2 + i * spacingZ;
                GLfloat currX = -w/2 + j * spacingX;

                vertices.push_back(currX);
                vertices.push_back(0);
                vertices.push_back(currZ);
            }
        }

        // for(int i = 0; i < vertices.size()/3; i++) {
        //     std::cout << vertices.at(3*i) << vertices.at(3*i+1) << vertices.at(3*i+2) << std::endl;
        // }

        GLuint* indices = new GLuint[width * depth * 6];
        GLuint count = 0;
        for (int i = 0; i < depth; i++) {
            for(int j = 0; j < width; j++) {
                indices[count++] = getIndex(j, i);
                indices[count++] = getIndex(j+1, i);
                indices[count++] = getIndex(j, i+1);

                indices[count++] = getIndex(j+1, i);
                indices[count++] = getIndex(j, i+1);
                indices[count++] = getIndex(j+1, i+1);
            }
        }
        // std::cout << getIndex(0, 0) << getIndex(1, 0) << getIndex(0, 1);

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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * width * depth * 6, indices, GL_STATIC_DRAW);

        // vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // delete the vertex and index buffers
        delete[] indices;
        vertices.clear();

        assert(glGetError()== GL_NO_ERROR);
    }

    GLuint getIndex(int i, int j) {
        return i + j * (width + 1);
    }

    void nextFrame() {
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, width * depth * 6, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(0);
    }
};

#endif