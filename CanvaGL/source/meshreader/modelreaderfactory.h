#include "objreader.h"

class ModelReaderFactory {
public:
    static std::unique_ptr<ModelReader> createModelReader(const std::string& fileType) {
        if (fileType == "obj") {
            return std::make_unique<OBJModelReader>();
        }
        // Add more conditions for different file types if needed
        return nullptr;
    }
};
