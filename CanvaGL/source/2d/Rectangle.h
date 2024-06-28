#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../graphicalobject.h"
#include <vector>

class Rectangle : public GraphicalObject {
    float width, height;
    glm::vec3 normal;
public:
    Rectangle(float w, float h) : width(w), height(h) {
        name = "rectangle";

        // Define the vertices and indices for the rectangle
        std::vector<GLfloat> vertices = {
            -width / 2.0f,  height / 2.0f, 0.0f, // Top-left
             width / 2.0f,  height / 2.0f, 0.0f, // Top-right
             width / 2.0f, -height / 2.0f, 0.0f, // Bottom-right
            -width / 2.0f, -height / 2.0f, 0.0f  // Bottom-left
        };

        // compute the normal of the rectangle
        glm::vec3 v0 = glm::vec3(vertices[0], vertices[1], vertices[2]);
        glm::vec3 v1 = glm::vec3(vertices[3], vertices[4], vertices[5]);
        normal = glm::normalize(glm::cross(v0, v1));

        GLuint indices[] = {
            0, 2, 1, 
            0, 3, 2
        };

        GLuint EBO, VBO;
        // Create VAO, EBO, VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);

        // Bind VAO
        glBindVertexArray(VAO);

        // Bind and set VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        // Bind and set EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        // Check for OpenGL errors
        assert(glGetError() == GL_NO_ERROR);
    }

    void nextFrame() override {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif // RECTANGLE_H
