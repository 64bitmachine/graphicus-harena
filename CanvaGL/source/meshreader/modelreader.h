#include <iostream>
#include <vector>
#include <GL/glew.h>

class ModelReader {
public:
    std::vector<GLfloat>* vertices = nullptr;
    std::vector<GLuint>* indices = nullptr;
    virtual bool loadModel(const std::string& filePath) = 0;
    virtual ~ModelReader() {
        // Clean up
        if (vertices != nullptr) {
            vertices->clear();
            indices->clear();
            delete vertices;
            delete indices;
        }
        // log
        std::cout << "ModelReader destroyed" << std::endl;
    }
};
