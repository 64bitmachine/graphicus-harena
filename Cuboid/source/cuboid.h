#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "line.h"
#include "shader.h"

class Cuboid {
private:
    glm::vec3 center;
    float length, breadth, height;
    glm::vec3 upVector;
    glm::vec3 rightVector;

    LineSegment *upAxis, *rightAxis, *frontAxis;

    GLuint VAO, VBO;
    std::vector<GLfloat> vertices;

    Shader* lineShader;

public:
    Cuboid(glm::vec3 center, float length, float breadth, float height, glm::vec3 upVector, glm::vec3 rightVector) :
        center(center), length(length), breadth(breadth), height(height), upVector(upVector), rightVector(rightVector) {
        // Calculate vertices based on center, length, breadth, and height
        calculateVertices();

        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate VBO and buffer data
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        int lengthAxis = 4;

        lineShader = new Shader("shaders/line.vs", "shaders/line.fs");

        // Initialize upAxis, rightAxis, and frontAxis
        upAxis = new LineSegment(lengthAxis, upVector, center, glm::vec3(1.0f, 0.0f, 0.0f));
        rightAxis = new LineSegment(lengthAxis, rightVector, center, glm::vec3(0.0f, 1.0f, 0.0f));
        frontAxis = new LineSegment(lengthAxis, glm::cross(upVector, rightVector), center, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    ~Cuboid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Bind VAO and draw
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        glBindVertexArray(0);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        lineShader->use();

        // Set projection and view matrices
        lineShader->setMat4("projection", projectionMatrix);
        lineShader->setMat4("view", viewMatrix);

        // Render upAxis, rightAxis, and frontAxis
        upAxis->render();
        rightAxis->render();
        frontAxis->render();
    }

    // Method to calculate vertices based on center, length, breadth, and height
    void calculateVertices() {
        // Calculate half lengths
        float halfLength = length / 2.0f;
        float halfBreadth = breadth / 2.0f;
        float halfHeight = height / 2.0f;

        // Calculate vertices
        glm::vec3 frontBottomLeft = center - upVector * halfHeight - rightVector * halfLength - glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 frontBottomRight = center - upVector * halfHeight + rightVector * halfLength - glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 frontTopRight = center + upVector * halfHeight + rightVector * halfLength - glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 frontTopLeft = center + upVector * halfHeight - rightVector * halfLength - glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 backBottomLeft = center - upVector * halfHeight - rightVector * halfLength + glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 backBottomRight = center - upVector * halfHeight + rightVector * halfLength + glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 backTopRight = center + upVector * halfHeight + rightVector * halfLength + glm::cross(upVector, rightVector) * halfBreadth;
        glm::vec3 backTopLeft = center + upVector * halfHeight - rightVector * halfLength + glm::cross(upVector, rightVector) * halfBreadth;

        // Push vertices into vector
        vertices = {
            // Front face
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z,
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            // Right face
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z,
            backBottomRight.x, backBottomRight.y, backBottomRight.z,
            backTopRight.x, backTopRight.y, backTopRight.z,
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z,
            backTopRight.x, backTopRight.y, backTopRight.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z,
            // Back face
            backBottomRight.x, backBottomRight.y, backBottomRight.z,
            backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            backTopLeft.x, backTopLeft.y, backTopLeft.z,
            backBottomRight.x, backBottomRight.y, backBottomRight.z,
            backTopLeft.x, backTopLeft.y, backTopLeft.z,
            backTopRight.x, backTopRight.y, backTopRight.z,
            // Left face
            backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            backTopLeft.x, backTopLeft.y, backTopLeft.z,
            // Top face
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z,
            backTopRight.x, backTopRight.y, backTopRight.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            backTopRight.x, backTopRight.y, backTopRight.z,
            backTopLeft.x, backTopLeft.y, backTopLeft.z,
            // Bottom face
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            backBottomRight.x, backBottomRight.y, backBottomRight.z,
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            backBottomRight.x, backBottomRight.y, backBottomRight.z,
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z
        };
    }
};
