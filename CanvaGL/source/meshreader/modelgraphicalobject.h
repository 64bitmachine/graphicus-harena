#ifndef MODELGRAPHICALOBJECT_H
#define MODELGRAPHICALOBJECT_H

#include "../graphicalobject.h"

class ModelGraphicalObject: public GraphicalObject {
public:
    GLuint indexCount;

    void nextFrame() override {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };
};

#endif