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
    glm::mat4* originalModelMat = new glm::mat4(1.0f);
    bool wireframe;

    GLuint VBO;
    std::vector<GLfloat> vertices;

    #define FRONT_BOTTOM_LEFT center - (upVector * height + rightVector * breadth + glm::cross(upVector, rightVector) * length) / 2.0f
    #define FRONT_BOTTOM_RIGHT center - (upVector * height - rightVector * breadth + glm::cross(upVector, rightVector) * length) / 2.0f
    #define FRONT_TOP_RIGHT center + (upVector * height + rightVector * breadth - glm::cross(upVector, rightVector) * length) / 2.0f
    #define FRONT_TOP_LEFT center + (upVector * height - rightVector * breadth - glm::cross(upVector, rightVector) * length) / 2.0f
    #define BACK_BOTTOM_LEFT center - (upVector * height + rightVector * breadth - glm::cross(upVector, rightVector) * length) / 2.0f
    #define BACK_BOTTOM_RIGHT center - (upVector * height - rightVector * breadth - glm::cross(upVector, rightVector) * length) / 2.0f
    #define BACK_TOP_RIGHT center + (upVector * height + rightVector * breadth + glm::cross(upVector, rightVector) * length) / 2.0f
    #define BACK_TOP_LEFT center + (upVector * height - rightVector * breadth + glm::cross(upVector, rightVector) * length) / 2.0f

public:
    Cuboid(glm::vec3 center, glm::vec3 dims, glm::vec3 upVector, glm::vec3 rightVector, bool wireframe = false) :
        center(center), length(dims.x), breadth(dims.y), height(dims.z), upVector(upVector), rightVector(rightVector), wireframe(wireframe) {
        
        name = "cuboid";
        
        // Calculate vertices based on center, length, breadth, and height
        if (wireframe) calculateEdges();
        else calculateVerticesTriangles();

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
        if (wireframe)
            glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        else 
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        glBindVertexArray(0);

        // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    // Method to calculate vertices based on center, length, breadth, and height
    void calculateVerticesTriangles() {
        // Calculate half lengths
        float halfLength = length / 2.0f;
        float halfBreadth = breadth / 2.0f;
        float halfHeight = height / 2.0f;

        // Calculate vertices
        glm::vec3 frontBottomLeft = FRONT_BOTTOM_LEFT;
        glm::vec3 frontBottomRight = FRONT_BOTTOM_RIGHT;
        glm::vec3 frontTopRight = FRONT_TOP_RIGHT;
        glm::vec3 frontTopLeft = FRONT_TOP_LEFT;
        glm::vec3 backBottomLeft = BACK_BOTTOM_LEFT;
        glm::vec3 backBottomRight = BACK_BOTTOM_RIGHT;
        glm::vec3 backTopRight = BACK_TOP_RIGHT;
        glm::vec3 backTopLeft = BACK_TOP_LEFT;

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

    void calculateEdges() {
        // Calculate half lengths
        float halfLength = length / 2.0f;
        float halfBreadth = breadth / 2.0f;
        float halfHeight = height / 2.0f;

        // Calculate vertices
        glm::vec3 frontBottomLeft = FRONT_BOTTOM_LEFT;
        glm::vec3 frontBottomRight = FRONT_BOTTOM_RIGHT;
        glm::vec3 frontTopRight = FRONT_TOP_RIGHT;
        glm::vec3 frontTopLeft = FRONT_TOP_LEFT;
        glm::vec3 backBottomLeft = BACK_BOTTOM_LEFT;
        glm::vec3 backBottomRight = BACK_BOTTOM_RIGHT;
        glm::vec3 backTopRight = BACK_TOP_RIGHT;
        glm::vec3 backTopLeft = BACK_TOP_LEFT;

        // Now, we need to define the edges of the cuboid using the vertices
        // We'll store the edge vertices in an array or vector

        // Define the edges using pairs of vertices
        vertices = {
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z, frontBottomRight.x, frontBottomRight.y, frontBottomRight.z,
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z, frontTopRight.x, frontTopRight.y, frontTopRight.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z, frontTopLeft.x, frontTopLeft.y, frontTopLeft.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z, frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z,
            backBottomLeft.x, backBottomLeft.y, backBottomLeft.z, backBottomRight.x, backBottomRight.y, backBottomRight.z,
            backBottomRight.x, backBottomRight.y, backBottomRight.z, backTopRight.x, backTopRight.y, backTopRight.z,
            backTopRight.x, backTopRight.y, backTopRight.z, backTopLeft.x, backTopLeft.y, backTopLeft.z,
            backTopLeft.x, backTopLeft.y, backTopLeft.z, backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            frontBottomLeft.x, frontBottomLeft.y, frontBottomLeft.z, backBottomLeft.x, backBottomLeft.y, backBottomLeft.z,
            frontBottomRight.x, frontBottomRight.y, frontBottomRight.z, backBottomRight.x, backBottomRight.y, backBottomRight.z,
            frontTopRight.x, frontTopRight.y, frontTopRight.z, backTopRight.x, backTopRight.y, backTopRight.z,
            frontTopLeft.x, frontTopLeft.y, frontTopLeft.z, backTopLeft.x, backTopLeft.y, backTopLeft.z
        };

        // print vertices computed
        std::cout << "Vertices computed: " << std::endl;
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

    void rescale(glm::vec3 relativeVec, bool override) {
        
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

        // std::cout << "Scale X: " << scaleX << std::endl;
        // std::cout << "Scale Y: " << scaleY << std::endl;
        // std::cout << "Scale Z: " << scaleZ << std::endl;

        // rescaling matrix
        glm::mat4 rescaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));


        if (override) {
            center += (relativeVec / 2.0f);
            length = newLength;
            breadth = newBreadth;
            height = newHeight;
        }
        glm::vec3 tempCenter = center + (relativeVec / 2.0f);

        if (override) {
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);
            *originalModelMat = translateBack * rescaleMat * translateToCenter * (*modelMat);
            *modelMat = *originalModelMat;

            for (GraphicalObject* child : children) {
                *child->modelMat = translateBack * translateToCenter * (*child->modelMat);
            }
        }
        else{
            // move back to original position
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), tempCenter);
            *modelMat = translateBack * rescaleMat * translateToCenter * (*originalModelMat);
        }


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
        glm::vec3 leftBottomFront = FRONT_BOTTOM_LEFT;
        glm::vec3 leftTopFront = FRONT_TOP_LEFT;
        glm::vec3 leftBottomBack = BACK_BOTTOM_LEFT;
        glm::vec3 leftTopBack = BACK_TOP_LEFT;

        // Output left face vertices
        std::cout << "Left Bottom Front: (" << leftBottomFront.x << ", " << leftBottomFront.y << ", " << leftBottomFront.z << ")" << std::endl;
        std::cout << "Left Top Front: (" << leftTopFront.x << ", " << leftTopFront.y << ", " << leftTopFront.z << ")" << std::endl;
        std::cout << "Left Bottom Back: (" << leftBottomBack.x << ", " << leftBottomBack.y << ", " << leftBottomBack.z << ")" << std::endl;
        std::cout << "Left Top Back: (" << leftTopBack.x << ", " << leftTopBack.y << ", " << leftTopBack.z << ")" << std::endl;
    }

    void rotateAboutUpVec() {
        // std::cout << "Rotate Up" << std::endl;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), upVector);

        // Rotate right vector and forward vector using rotation matrix
        rightVector = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(rightVector, 0.0f)));

        // update model matrix

        // translate to center
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), -center);

        // translate back to original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);

        *modelMat = translateBack * rotationMatrix * translateToCenter * (*modelMat);
        *originalModelMat = *modelMat;

        for (GraphicalObject* child : children) {
            *child->modelMat = translateBack * rotationMatrix * translateToCenter * (*child->modelMat);
        }
    }

    glm::vec3 getCorner(int index) {
        switch (index) {
            case 0:
                return FRONT_BOTTOM_LEFT;
            case 1:
                return FRONT_BOTTOM_RIGHT;
            case 2:
                return FRONT_TOP_LEFT;
            case 3:
                return FRONT_TOP_RIGHT;
            case 4:
                return BACK_BOTTOM_LEFT;
            case 5:
                return BACK_BOTTOM_RIGHT;
            case 6:
                return BACK_TOP_LEFT;
            case 7:
                return BACK_TOP_RIGHT;
            default:
                return glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }

    void rescaleUsingCorner(glm::vec3 drag, bool override) {
        int activeCorner = 3;

        // adjCorner using xor 1, 2, 4
        int cornerB = activeCorner ^ 1;
        int cornerH = activeCorner ^ 2;
        int cornerL = activeCorner ^ 4;

        std::cout << "Active Corner: " << activeCorner << std::endl;
        std::cout << "Corner B: " << cornerB << std::endl;
        std::cout << "Corner H: " << cornerH << std::endl;
        std::cout << "Corner L: " << cornerL << std::endl;

        glm::vec3 cornerBVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerB));
        glm::vec3 cornerHVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerH));
        glm::vec3 cornerLVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerL));

        float deltaB = glm::dot(cornerBVec, drag);
        float deltaH = glm::dot(cornerHVec, drag);
        float deltaL = glm::dot(cornerLVec, drag);

        rescale(glm::vec3(deltaB, deltaH, deltaL), override);
    }
};
