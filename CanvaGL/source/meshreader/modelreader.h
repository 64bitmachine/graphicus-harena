#ifndef MODELREADER_H
#define MODELREADER_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "modelgraphicalobject.h"

class ModelReader {
public:
    std::vector<GLfloat>* vertices = nullptr;
    std::vector<GLuint>* indices = nullptr;
    virtual std::unique_ptr<GraphicalObject> loadModel() = 0;
    virtual void initModelGO() = 0;
    std::unique_ptr<GraphicalObject> modelGO = nullptr;
    std::string filePath;

    ModelReader(std::string filePath): filePath(filePath) {}

    virtual ~ModelReader() {
        // Clean up
        if (vertices != nullptr) {
            vertices->clear();
            indices->clear();
            delete vertices;
            delete indices;
        }
    }
};

#endif