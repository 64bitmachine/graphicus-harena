#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <glm/glm.hpp>

class Particle : public GraphicalObject {
public:
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 acceleration;
    GLuint VBO;

    Particle(float x, float y, float z): 
        position(x, y, z),
        prevPosition(x, y, z),
        acceleration(0.0f) {
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

    void applyForce(const glm::vec3& force) {
        acceleration += force;
    }

    void update(float timeStep) {
        // verlet integration
        glm::vec3 velocity = position - prevPosition;
        prevPosition = position;
        position += velocity + acceleration * timeStep * timeStep;
        acceleration = glm::vec3(0.0f);
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