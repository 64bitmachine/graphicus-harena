#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "line.h"
#include "graphicalobject.h"

class Cuboid : public GraphicalObject {
private:
    glm::vec3 center;
    float length, breadth, height;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 sizeLimits = glm::vec3(0.25f, 0.25f, 0.25f);

    GLuint VBO;
    std::vector<GLfloat> vertices;

public:
    Cuboid(glm::vec3 center, glm::vec3 dims, glm::vec3 upVector, glm::vec3 rightVector) :
        center(center), length(dims.x), breadth(dims.y), height(dims.z), upVector(upVector), rightVector(rightVector) {
        
        name = "cuboid";
        
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

        // print the cuboid is created
        std::cout << "Cuboid created" << std::endl;
    }

    ~Cuboid() {
        glDeleteBuffers(1, &VBO);
    }

    void nextFrame() {

        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Bind VAO and draw
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        glBindVertexArray(0);

        // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    // Method to calculate vertices based on center, length, breadth, and height
    void calculateVertices() {
        // Calculate half lengths
        float halfLength = length / 2.0f;
        float halfBreadth = breadth / 2.0f;
        float halfHeight = height / 2.0f;

        // Calculate vertices
        glm::vec3 frontBottomLeft = center - upVector * halfHeight - rightVector * halfBreadth - glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 frontBottomRight = center - upVector * halfHeight + rightVector * halfBreadth - glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 frontTopRight = center + upVector * halfHeight + rightVector * halfBreadth - glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 frontTopLeft = center + upVector * halfHeight - rightVector * halfBreadth - glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 backBottomLeft = center - upVector * halfHeight - rightVector * halfBreadth + glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 backBottomRight = center - upVector * halfHeight + rightVector * halfBreadth + glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 backTopRight = center + upVector * halfHeight + rightVector * halfBreadth + glm::cross(upVector, rightVector) * halfLength;
        glm::vec3 backTopLeft = center + upVector * halfHeight - rightVector * halfBreadth + glm::cross(upVector, rightVector) * halfLength;

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

        // print vertices computed
        std::cout << "Vertices computed" << std::endl;
    }

    void moveTo(glm::vec3 p) {
        center = p;
    }

    glm::vec3 getUpVector() {
        return upVector;
    }

    glm::vec3 getRightVector() {
        return rightVector;
    }

    glm::vec3 getCenter() {
        return center;
    }

    void rescale(glm::vec3 relativeVec) {
        
        if (relativeVec == glm::vec3(0.0f)) { return; }

        float newLength, newBreadth, newHeight;
        // update the relativeVec to remove invalid scaling
        if (length + relativeVec.z < sizeLimits[0]) {
            newLength = sizeLimits[0];
            relativeVec.z = newLength - length;
        } else {
            newLength = length + relativeVec.z;
        }
        
        if (breadth + relativeVec.x < sizeLimits[1]) {
            newBreadth = sizeLimits[1];
            relativeVec.x = newBreadth - breadth;
        } else {
            newBreadth = breadth + relativeVec.x;
        }

        if (height + relativeVec.y < sizeLimits[2]) {
            newHeight = sizeLimits[2];
            relativeVec.y = newHeight - height;
        } else {
            newHeight = height + relativeVec.y;
        }

        // translation matrix
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), -center);

        float scaleX = newBreadth / breadth;
        float scaleY = newHeight / height;
        float scaleZ = newLength / length;

        std::cout << "Scale X: " << scaleX << std::endl;
        std::cout << "Scale Y: " << scaleY << std::endl;
        std::cout << "Scale Z: " << scaleZ << std::endl;

        // rescaling matrix
        glm::mat4 rescaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));


        center += (relativeVec / 2.0f);

        // move back to original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);

        length = newLength;
        breadth = newBreadth;
        height = newHeight;

        *modelMat = translateBack * rescaleMat * translateToCenter * (*modelMat);

        // printLeftFaceVertices();

        // // print rescaling matrix
        // for(int i = 0; i < 4; i++) {
        //     for(int j = 0; j < 4; j++) {
        //         std::cout << (*modelMat)[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        // glm::vec3 leftBottomFront = center - (breadth * rightVector + height * upVector + length * glm::cross(rightVector, upVector))/2.0f;
        // glm::vec4 newVec = (*modelMat) * glm::vec4(leftBottomFront, 1.0f);
        // std::cout << "New Left Bottom Front: " << newVec.x << ", " << newVec.y << ", " << newVec.z << std::endl;
    }

    void printLeftFaceVertices() {
        std::cout << "Left Face Vertices" << std::endl;

        // Calculate half lengths
        float halfLength = length / 2.0f;
        float halfBreadth = breadth / 2.0f;
        float halfHeight = height / 2.0f;

        // Calculate left face vertices
        glm::vec3 leftBottomFront = center - halfBreadth * rightVector - halfHeight * upVector - halfLength * glm::cross(rightVector, upVector);
        glm::vec3 leftTopFront = center - halfBreadth * rightVector + halfHeight * upVector - halfLength * glm::cross(rightVector, upVector);
        glm::vec3 leftBottomBack = center - halfBreadth * rightVector - halfHeight * upVector + halfLength * glm::cross(rightVector, upVector);
        glm::vec3 leftTopBack = center - halfBreadth * rightVector + halfHeight * upVector + halfLength * glm::cross(rightVector, upVector);

        // Output left face vertices
        std::cout << "Left Bottom Front: (" << leftBottomFront.x << ", " << leftBottomFront.y << ", " << leftBottomFront.z << ")" << std::endl;
        std::cout << "Left Top Front: (" << leftTopFront.x << ", " << leftTopFront.y << ", " << leftTopFront.z << ")" << std::endl;
        std::cout << "Left Bottom Back: (" << leftBottomBack.x << ", " << leftBottomBack.y << ", " << leftBottomBack.z << ")" << std::endl;
        std::cout << "Left Top Back: (" << leftTopBack.x << ", " << leftTopBack.y << ", " << leftTopBack.z << ")" << std::endl;
    }

};
