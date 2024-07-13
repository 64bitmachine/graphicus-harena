#include "objreader.h"

class ModelReaderFactory {
public:
    static std::unique_ptr<ModelReader> createModelReader(const std::string& filePath) {
        std::string fileType = filePath.substr(filePath.find_last_of(".") + 1);
        if (fileType == "obj") {
            return std::make_unique<OBJModelReader>(filePath);
        }
        // Add more conditions for different file types if needed
        return nullptr;
    }
};
