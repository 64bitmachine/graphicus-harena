#include "modelreader.h"

class OBJModelReader : public ModelReader {
public:
    bool loadModel(const std::string& filePath) override {
        // Implementation for reading an OBJ file
        std::cout << "Loading OBJ model from " << filePath << std::endl;
        // Load and parse the OBJ file here
        throw std::runtime_error("Not implemented yet");
        return true;
    }
};
