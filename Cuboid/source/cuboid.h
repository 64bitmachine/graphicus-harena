#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "line.h"
#include "graphicalobject.h"

// macro to get normalized cross
#define NORM_CROSS(a, b) glm::normalize(glm::cross(a, b))

class Cuboid : public GraphicalObject {
private:
    glm::vec3 center;
    float length, breadth, height;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 sizeLimits = glm::vec3(0.25f, 0.25f, 0.25f);
    glm::mat4* originalModelMat = new glm::mat4(1.0f);
    bool wireframe;
    glm::vec3 rotationAngles = glm::vec3(0.0f, 0.0f, 0.0f);

    GLuint VBO;
    std::vector<GLfloat> vertices;

    #define FRONT_BOTTOM_LEFT center - ((upVector * height + rightVector * breadth + NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define FRONT_BOTTOM_RIGHT center - ((upVector * height - rightVector * breadth + NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define FRONT_TOP_RIGHT center + ((upVector * height + rightVector * breadth - NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define FRONT_TOP_LEFT center + ((upVector * height - rightVector * breadth - NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define BACK_BOTTOM_LEFT center - ((upVector * height + rightVector * breadth - NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define BACK_BOTTOM_RIGHT center - ((upVector * height - rightVector * breadth - NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define BACK_TOP_RIGHT center + ((upVector * height + rightVector * breadth + NORM_CROSS(upVector, rightVector) * length) / 2.0f)
    #define BACK_TOP_LEFT center + ((upVector * height - rightVector * breadth + NORM_CROSS(upVector, rightVector) * length) / 2.0f)

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

        if (wireframe) createAxes();
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

    void calculateEdges(glm::vec3 c, glm::vec3 up, glm::vec3 right, float l, float b, float h) {
        // Calculate half lengths
        float halfLength = l / 2.0f;
        float halfBreadth = b / 2.0f;
        float halfHeight = h / 2.0f;

        // Calculate vertices of the cuboid's corners
        glm::vec3 frontBottomLeft = c - (up * halfHeight) - (right * halfBreadth) - glm::cross(up, right) * halfLength;
        glm::vec3 frontBottomRight = c - (up * halfHeight) + (right * halfBreadth) - glm::cross(up, right) * halfLength;
        glm::vec3 frontTopRight = c + (up * halfHeight) + (right * halfBreadth) - glm::cross(up, right) * halfLength;
        glm::vec3 frontTopLeft = c + (up * halfHeight) - (right * halfBreadth) - glm::cross(up, right) * halfLength;
        glm::vec3 backBottomLeft = c - (up * halfHeight) - (right * halfBreadth) + glm::cross(up, right) * halfLength;
        glm::vec3 backBottomRight = c - (up * halfHeight) + (right * halfBreadth) + glm::cross(up, right) * halfLength;
        glm::vec3 backTopRight = c + (up * halfHeight) + (right * halfBreadth) + glm::cross(up, right) * halfLength;
        glm::vec3 backTopLeft = c + (up * halfHeight) - (right * halfBreadth) + glm::cross(up, right) * halfLength;

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

        createAxes();
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

    void rescale(glm::vec3 sizeDiffVec, bool override) {
        printCuboidState();
        if (sizeDiffVec == glm::vec3(0.0f)) { return; }

        float newLength, newBreadth, newHeight;
        // update the relativeVec to remove invalid scaling
        if (length + sizeDiffVec.z < sizeLimits[0]) {
            newLength = sizeLimits[0];
            sizeDiffVec.z = newLength - length;
        } else {
            newLength = length + sizeDiffVec.z;
        }
        
        if (breadth + sizeDiffVec.x < sizeLimits[1]) {
            newBreadth = sizeLimits[1];
            sizeDiffVec.x = newBreadth - breadth;
        } else {
            newBreadth = breadth + sizeDiffVec.x;
        }

        if (height + sizeDiffVec.y < sizeLimits[2]) {
            newHeight = sizeLimits[2];
            sizeDiffVec.y = newHeight - height;
        } else {
            newHeight = height + sizeDiffVec.y;
        }

        glm::vec3 cuboidOrientation = getCuboidOrientation();
        glm::mat4 rotateToCuboidOrientationX = glm::rotate(glm::mat4(1.0f), -glm::radians(cuboidOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotateToCuboidOrientationY = glm::rotate(glm::mat4(1.0f), -glm::radians(cuboidOrientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotateToCuboidOrientationZ = glm::rotate(glm::mat4(1.0f), -glm::radians(cuboidOrientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotateToCuboidOrientation = rotateToCuboidOrientationX * rotateToCuboidOrientationY * rotateToCuboidOrientationZ;

        // revert rotation
        glm::mat4 revertRotX = glm::rotate(glm::mat4(1.0f), glm::radians(cuboidOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 revertRotY = glm::rotate(glm::mat4(1.0f), glm::radians(cuboidOrientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 revertRotZ = glm::rotate(glm::mat4(1.0f), glm::radians(cuboidOrientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 revertRot = revertRotX * revertRotY * revertRotZ;


        // translation matrix
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), -center);

        float scaleX = (newBreadth / breadth);
        float scaleY = (newHeight / height);
        float scaleZ = (newLength / length);

        // rescaling matrix
        glm::mat4 rescaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));

        glm::vec3 dc = (rightVector * sizeDiffVec.x + upVector * sizeDiffVec.y + glm::cross(rightVector, upVector) * sizeDiffVec.z) / 2.0f;
 
        if (override) {
            center += dc;
            length = newLength;
            breadth = newBreadth;
            height = newHeight;

            // glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);
            // *modelMat = translateBack * revertRot * rescaleMat * rotateToCuboidOrientation * translateToCenter * (*originalModelMat);
            // *originalModelMat = *modelMat;

            // for (GraphicalObject* child : children) {
            //     *child->modelMat = translateBack * translateToCenter * (*child->modelMat);
            // }
            calculateEdges(center, upVector, rightVector, newLength, newBreadth, newHeight);
        }
        else{
            glm::vec3 tempCenter = center + dc;
            // move back to original position
            // glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), tempCenter);
            // *modelMat = translateBack * revertRot * rescaleMat * rotateToCuboidOrientation * translateToCenter * (*originalModelMat);

            calculateEdges(tempCenter, upVector, rightVector, newLength, newBreadth, newHeight);
        }

        printCuboidState();
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

    /**
     * @brief 
     * 
     * @param rotateMode 0 - Up, 1 - Right, 3 - Right x Up
     */
    void rotate(int rotateMode) {
        // std::cout << "Rotate Up" << std::endl;
        glm::mat4 rotationMatrix;
        if (rotateMode == 0) {
            rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.01f, upVector);
            rotationAngles += glm::vec3(0.0f, 0.01f, 0.0f);
        }
        else if (rotateMode == 1) {
            rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.01f, rightVector);
            rotationAngles += glm::vec3(0.01f, 0.0f, 0.0f);
        }
        else if (rotateMode == 2) {
            rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.01f, glm::cross(rightVector, upVector));
            rotationAngles += glm::vec3(0.0f, 0.0f, 0.01f);
        }

        // Rotate right vector and forward vector using rotation matrix
        glm::vec4 newRightVec = rotationMatrix * glm::vec4(rightVector, 1.0f);
        rightVector = glm::normalize(glm::vec3(newRightVec.x, newRightVec.y, newRightVec.z));
        glm::vec4 newUpVec = rotationMatrix * glm::vec4(upVector, 1.0f);
        upVector = glm::normalize(glm::vec3(newUpVec.x, newUpVec.y, newUpVec.z));

        // update model matrix

        // translate to center
        glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), -center);

        // translate back to original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);

        // *modelMat = translateBack * rotationMatrix * translateToCenter * (*modelMat);
        // *originalModelMat = *modelMat;

        // for (GraphicalObject* child : children) {
        //     *child->modelMat = translateBack * rotationMatrix * translateToCenter * (*child->modelMat);
        // }

        children.clear();

        calculateEdges(center, upVector, rightVector, length, breadth, height);

        printCuboidOrientation();
    }

    void createAxes() {

        children.clear();

        int lengthAxis = 4;

        Shader *lineShader = new Shader("shaders/line.vs", "shaders/line.fs");
        LineSegment *upAxis, *rightAxis, *frontAxis;

        // Initialize upAxis, rightAxis, and frontAxis
        upAxis = new LineSegment(lengthAxis, getUpVector(), getCenter(), glm::vec3(1.0f, 0.0f, 0.0f), "up");
        rightAxis = new LineSegment(lengthAxis, getRightVector(), getCenter(), glm::vec3(0.0f, 1.0f, 0.0f), "right");
        frontAxis = new LineSegment(lengthAxis, 
        glm::cross(getUpVector(), getRightVector()), getCenter(), glm::vec3(0.0f, 0.0f, 1.0f), "back");
        
        upAxis->setShader(lineShader);
        frontAxis->setShader(lineShader);
        rightAxis->setShader(lineShader);

        children.push_back(upAxis);
        children.push_back(rightAxis);
        children.push_back(frontAxis);
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

        // std::cout << "Active Corner: " << activeCorner << std::endl;
        // std::cout << "Corner B: " << cornerB << std::endl;
        // std::cout << "Corner H: " << cornerH << std::endl;
        // std::cout << "Corner L: " << cornerL << std::endl;

        glm::vec3 cornerBVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerB));
        glm::vec3 cornerHVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerH));
        glm::vec3 cornerLVec = glm::normalize(getCorner(activeCorner) - getCorner(cornerL));

        float deltaB = glm::dot(cornerBVec, drag);
        float deltaH = glm::dot(cornerHVec, drag);
        float deltaL = glm::dot(cornerLVec, drag);

        std::cout << "Delta B: " << deltaB << std::endl;
        std::cout << "Delta H: " << deltaH << std::endl;
        std::cout << "Delta L: " << deltaL << std::endl;

        rescale(glm::vec3(deltaB, deltaH, deltaL), override);
        glm::vec3 zeroVertex = getCorner(0);
        std::cout << "Zero Vertex: (" << zeroVertex.x << ", " << zeroVertex.y << ", " << zeroVertex.z << ")" << std::endl;
    }

    void printCuboidState() {
        std::cout << "============= Cuboid State ============" << std::endl;
        std::cout << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
        std::cout << "Right: (" << rightVector.x << ", " << rightVector.y << ", " << rightVector.z << ")" << std::endl;
        std::cout << "Up: (" << upVector.x << ", " << upVector.y << ", " << upVector.z << ")" << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Breadth: " << breadth << std::endl;
        std::cout << "Length: " << length << std::endl;

        float l1, l2, l3, l4, b1, b2, b3, b4, h1, h2, h3, h4;
        l1 = glm::distance(getCorner(5), getCorner(1));
        l2 = glm::distance(getCorner(0), getCorner(4));
        l3 = glm::distance(getCorner(3), getCorner(7));
        l4 = glm::distance(getCorner(2), getCorner(6));

        std::cout << "L1: " << l1 << ", L2: " << l2 << ", L3: " << l3 << ", L4: " << l4 << std::endl;

        b1 = glm::distance(getCorner(1), getCorner(0));
        b2 = glm::distance(getCorner(2), getCorner(3));
        b3 = glm::distance(getCorner(4), getCorner(5));
        b4 = glm::distance(getCorner(6), getCorner(7));

        std::cout << "B1: " << b1 << ", B2: " << b2 << ", B3: " << b3 << ", B4: " << b4 << std::endl;

        h1 = glm::distance(getCorner(0), getCorner(2));
        h2 = glm::distance(getCorner(1), getCorner(3));
        h3 = glm::distance(getCorner(4), getCorner(6));
        h4 = glm::distance(getCorner(5), getCorner(7));

        std::cout << "H1: " << h1 << ", H2: " << h2 << ", H3: " << h3 << ", H4: " << h4 << std::endl;

        for (int i = 0; i < 8; i++) {
            std::cout << "Corner " << i << ": (" << getCorner(i).x << ", " << getCorner(i).y << ", " << getCorner(i).z << ")" << std::endl;
        }

        // printVerticesUsingMatrix();
        // printCuboidOrientation();

        std::cout << "=======================================" << std::endl;
    }

    void printVerticesUsingMatrix() {
        glm::vec3 originalUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 originalRight = glm::vec3(1.0f, 0.0f, 0.0f);
        float originalL = 2;
        float originalB = 1;
        float originalH = 1.5;
        // vertex 0 front bottom left
        glm::vec4 v0 = (*modelMat) * glm::vec4((- originalRight * originalB - originalUp * originalH + glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 1 front bottom right
        glm::vec4 v1 = (*modelMat) * glm::vec4((+ originalRight * originalB - originalUp * originalH + glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 2 front top left
        glm::vec4 v2 = (*modelMat) * glm::vec4((- originalRight * originalB + originalUp * originalH + glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 3 front top right
        glm::vec4 v3 = (*modelMat) * glm::vec4((+ originalRight * originalB + originalUp * originalH + glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 4 back bottom left
        glm::vec4 v4 = (*modelMat) * glm::vec4((- originalRight * originalB - originalUp * originalH - glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 5 back bottom right
        glm::vec4 v5 = (*modelMat) * glm::vec4((+ originalRight * originalB - originalUp * originalH - glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 6 back top left
        glm::vec4 v6 = (*modelMat) * glm::vec4((- originalRight * originalB + originalUp * originalH - glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);
        // vertex 7 back top right
        glm::vec4 v7 = (*modelMat) * glm::vec4((+ originalRight * originalB + originalUp * originalH - glm::cross(originalRight, originalUp) * originalL) / 2.0f, 1.0f);

        std::cout << "V0: (" << v0.x << ", " << v0.y << ", " << v0.z << ")" << std::endl;
        std::cout << "V1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
        std::cout << "V2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;
        std::cout << "V3: (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
        std::cout << "V4: (" << v4.x << ", " << v4.y << ", " << v4.z << ")" << std::endl;
        std::cout << "V5: (" << v5.x << ", " << v5.y << ", " << v5.z << ")" << std::endl;
        std::cout << "V6: (" << v6.x << ", " << v6.y << ", " << v6.z << ")" << std::endl;
        std::cout << "V7: (" << v7.x << ", " << v7.y << ", " << v7.z << ")" << std::endl;
    }

    glm::vec3 getCuboidOrientation() {
        // // (angle around X-axis)
        // float rotX = acos(glm::dot(upVector, glm::vec3(0.0f, 1.0f, 0.0f)));

        // // (angle around Y-axis)
        // float rotY = acos(glm::dot(rightVector, glm::vec3(1.0f, 0.0f, 0.0f)));

        // // (angle around Z-axis)
        // float rotZ = atan2(rightVector.y, rightVector.x);

        // // Convert angles from radians to degrees
        // rotX = glm::degrees(rotX);
        // rotY = glm::degrees(rotY);
        // rotZ = glm::degrees(rotZ);

        // return glm::vec3(rotX, rotY, rotZ);
        return rotationAngles;
    }

    void printCuboidOrientation() {
        glm::vec3 cuboidOrientation = getCuboidOrientation();
        std::cout << "(X-axis rotation): " << cuboidOrientation.x << " degrees" << std::endl;
        std::cout << "(Y-axis rotation): " << cuboidOrientation.y << " degrees" << std::endl;
        std::cout << "(Z-axis rotation): " << cuboidOrientation.z << " degrees" << std::endl;
    }

    void move(glm::vec3 p, bool override = false) {

        glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), p);

        if (override) {
            center += p;
            *modelMat = translateMat * (*originalModelMat);
            *originalModelMat = *modelMat;

            for(GraphicalObject* child : children) {
                *child->modelMat = translateMat * (*child->modelMat);
            }
        }
        else {
            *modelMat = translateMat * (*originalModelMat);
        }
    }

    void reset() {
        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), -rotationAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), -rotationAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), -rotationAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

        *modelMat = rotX * rotY * rotZ;
        *originalModelMat = *modelMat;
    }
};
