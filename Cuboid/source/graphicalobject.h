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
        Shader* shader;
        std::vector<GraphicalObject*> children;
        glm::mat4 *projMat, *viewMat;

        GraphicalObject() {}

        void addChild(GraphicalObject* child) {
            children.push_back(child);
        }

        virtual ~GraphicalObject() {
            glDeleteVertexArrays(1, &VAO);
        }
        void render() {
            shader->use();
            nextFrame();
        }

        virtual void nextFrame() = 0;

        const Shader* getShader() {
            return shader;
        }

        void setShader(Shader* s) {
            shader = s;
        }

        void setProjMat(glm::mat4* p) {
            projMat = p;
            shader->setMat4("projection", *projMat);
        }

        void setViewMat(glm::mat4* v) {
            viewMat = v;
            shader->setMat4("view", *viewMat);
        }
};

#endif