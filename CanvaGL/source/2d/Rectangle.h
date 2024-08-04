#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../graphicalobject.h"
#include <vector>

class Rectangle : public GraphicalObject {
    float width, height;
    glm::vec3 normal, position;
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
        position = glm::vec3(0.0f);

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
        shader->setFloat("alpha", 0.6f);
        shader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
        assert(glGetError() == GL_NO_ERROR);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void setNormal(glm::vec3 n) { 
        normal = glm::normalize(n);

        *modelMat = glm::translate(glm::mat4(1.0f), position) * getRotationMatrix(normal);
    }

    glm::mat4 getRotationMatrix(glm::vec3 normal) {
        // Normalize the normal vector
        normal = glm::normalize(normal);

        // Calculate the rotation axis
        glm::vec3 rotationAxis = glm::cross(normal, glm::vec3(0, 0, 1));

        // Calculate the rotation angle
        float rotationAngle = -acos(glm::dot(normal, glm::vec3(0, 0, 1)));

        // Create a rotation matrix
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);

        return rotationMatrix;
    }

    glm::vec3 getNormal() { return normal; }

    void setPosition(glm::vec3 p) { 
        position = p;

        *modelMat = glm::translate(glm::mat4(1.0f), position);
    }

    glm::vec3 getPosition() { return position; }

    void moveForward(float dist) {
        position += normal * dist;
        *modelMat = glm::translate(glm::mat4(1.0f), position) * getRotationMatrix(normal);
    }
};

#endif // RECTANGLE_H
