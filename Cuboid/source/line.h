#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class LineSegment {
private:
    float length;
    glm::vec3 direction;
    glm::vec3 startPoint;
    // color vec
    glm::vec3 color;
    GLuint VAO, VBO;
public:
    LineSegment(float l, const glm::vec3& dir, const glm::vec3& start, const glm::vec3& col) : length(l), direction(glm::normalize(dir)), startPoint(start), color(col) {
        
        // Initialize OpenGL buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind VAO
        glBindVertexArray(VAO);

        // Bind VBO and set data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, NULL, GL_STATIC_DRAW); // Only need space for two vertices

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Set color attribute pointer
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Unbind VAO
        glBindVertexArray(0);
    }

    ~LineSegment() {
        // Delete OpenGL buffers
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void render() {
        // Calculate endpoint
        glm::vec3 endPoint = startPoint + length * direction;

        // print(startPoint.x, startPoint.y, startPoint.z, endPoint.x, endPoint.y, endPoint.z);
        // std::cout << startPoint.x << " " << startPoint.y << " " << startPoint.z << " " << endPoint.x << " " << endPoint.y << " " << endPoint.z << std::endl;

        // Bind VAO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Update VBO data
        GLfloat vertices[] = {
            startPoint.x, startPoint.y, startPoint.z, color.x, color.y, color.z,
            endPoint.x, endPoint.y, endPoint.z, color.x, color.y, color.z
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Render line segment
        glDrawArrays(GL_LINES, 0, 2);

        // Unbind VAO
        glBindVertexArray(0);
    }
};
