#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <glm/glm.hpp>

class Particle : public GraphicalObject {
public:
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 acceleration;
    GLuint VBO;
    bool isPinned;
    int rowi, colj;

    Particle(float x, float y, float z, bool pinned): 
        position(x, y, z),
        prevPosition(x, y, z),
        acceleration(0.0f),
        isPinned(pinned) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind VAO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Update VBO data
        GLfloat vertices[] = {
            position.x, position.y, position.z
        };
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        // Vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);
    }

    void apply_force(const glm::vec3& force) {
        if (!isPinned) {
            acceleration += force;
        }
    }

    void update(float time_step) {
        // verlet intergration
        if (!isPinned) {
            glm::vec3 velocity = position - prevPosition;
            prevPosition = position;
            // std::cout << "velocity: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
            // position += velocity + acceleration * time_step * time_step;
            glm::vec3 deltaPos = velocity + acceleration * time_step * time_step;
            acceleration = glm::vec3(0.0f); // reset after update
            // print deltaPos
            // std::cout << "deltaPos: " << deltaPos.x << " " << deltaPos.y << " " << deltaPos.z << std::endl;
            *modelMat = glm::translate(*modelMat, deltaPos);
            position += deltaPos;
        }
    }

    void constrain_to_bounds(float width, float height) {
        if (position.y < 0) position.y = 0;
        if (position.y < height) position.y = height;
    }

    void nextFrame() {
        // Bind VAO
        glBindVertexArray(VAO);
        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, 1);

        // Unbind VAO
        glBindVertexArray(0);
    }
};


#endif // __PARTICLE_H__