#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include <string>

#ifndef GRAPHICALOBJECT_H
#define GRAPHICALOBJECT_H

class GraphicalObject {
    public:
        std::string name;
        GLuint VAO = 0;
        Shader* shader = nullptr;
        std::vector<GraphicalObject*> children;
        glm::mat4 *projMat, *viewMat, *modelMat;
        GLuint texture;

        GraphicalObject() {
            modelMat = new glm::mat4(1.0f);
            projMat = new glm::mat4(1.0f);
            viewMat = new glm::mat4(1.0f);
            texture = -1;
        }

        void addChild(GraphicalObject* child) {
            children.push_back(child);
        }

        virtual ~GraphicalObject() {
            glDeleteVertexArrays(1, &VAO);
        }
        void render(glm::mat4* projMat, glm::mat4* viewMat) {
            shader->use();
            shader->setMat4("projection", *projMat);
            shader->setMat4("view", *viewMat);
            shader->setMat4("model", *modelMat);
            nextFrame();

            for(GraphicalObject* child : children) {
                child->render(projMat, viewMat);
            }
        }

        virtual void nextFrame() = 0;

        const Shader* getShader() {
            return shader;
        }

        void setShader(Shader* s) {
            shader = s;
        }

        void setTexture(GLuint t) {
            texture = t;
        }
};

#endif