#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "graphicalobject.h"
#include <glm/gtc/matrix_transform.hpp>

class Scene {
    
    std::vector<GraphicalObject*> objects;
    glm::mat4 *projection, *view;

    public:

        Scene() {}
        
        ~Scene() {
            for(GraphicalObject* object : objects) {
                delete object;
            }
            objects.clear();
        }

        void add(GraphicalObject* object) {
            objects.push_back(object);
        }

        void render(glm::mat4* projection, glm::mat4* view) {
            for(GraphicalObject* object : objects) {
                object->setProjMat(projection);
                object->setViewMat(view);
                object->render();
            }
        }
};

#endif