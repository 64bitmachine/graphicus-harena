#include <iostream>

class ModelReader {
public:
    virtual bool loadModel(const std::string& filePath) = 0;
    virtual ~ModelReader() {}
};
