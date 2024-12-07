#ifndef __CONSTRAINT_H__
#define __CONSTRAINT_H__

#include "particle.h"
#include <cmath>
#include <limits>
#include <glm/glm.hpp>

class Constraint: public GraphicalObject {
public:
    Particle *p1;
    Particle *p2;
    float initial_length;
    bool active;
    GLuint VBO;
    int rowId, colId;
    bool isHor;

    Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2) {
        initial_length = std::hypot(p2->position.x - p1->position.x,
                                    p2->position.y - p1->position.y);
        active = true;
        isHor = false;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind VAO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Allocate buffer space
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
        
        // Vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);
    }

    void satisfy() {
        if (!active) return;

        glm::vec3 delta = p2->position - p1->position;
        float current_length = std::hypot(delta.x, delta.y);
        float difference = (current_length - initial_length) / current_length;
        glm::vec3 correction = delta * 0.5f * difference;

        if (!p1->isPinned) p1->position += correction;
        if (!p2->isPinned) p2->position -= correction;
    }

    void deactivate() {
        active = false;
    }

    void nextFrame() {
        if (!active) return;

        // Update vertex data
        GLfloat vertices[] = {
            p1->position.x, p1->position.y, p1->position.z,
            p2->position.x, p2->position.y, p2->position.z
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }

    void printPos() {
        std::cout << p1->position.x << " " << p1->position.y << " " << p1->position.z << std::endl;
        std::cout << p2->position.x << " " << p2->position.y << " " << p2->position.z << std::endl;
    }

};

#endif // CONSTRAINT_H