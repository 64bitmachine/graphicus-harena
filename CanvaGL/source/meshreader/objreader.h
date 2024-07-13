#include "modelreader.h"
#include <fstream>

class OBJModelReader : public ModelReader {
public:
    std::unique_ptr<GraphicalObject> loadModel() override {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return nullptr;
        }

        vertices = new std::vector<GLfloat>;
        indices = new std::vector<GLuint>;

        std::string line;
        while (std::getline(file, line)) {
            if (!processLine(line)) {
                std::cerr << "Failed to process line: " << line << std::endl;
                return nullptr;
            }
        }

        file.close();

        initModelGO();
        return std::move(modelGO);
    }

    OBJModelReader(std::string filePath): ModelReader(filePath) {
    }

    virtual ~OBJModelReader() {
    }

    virtual void initModelGO() override {
        ModelGraphicalObject* tempModelGO = new ModelGraphicalObject();

        // Create VAO
        glGenVertexArrays(1, &tempModelGO->VAO);
        GLuint VBO, EBO;
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);

        // Bind VAO
        glBindVertexArray(tempModelGO->VAO);

        // Bind VBO and upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), vertices->data(), GL_STATIC_DRAW);
        std::cout << "vertex size: " << vertices->size() << std::endl;

        // Bind EBO and upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), indices->data(), GL_STATIC_DRAW);
        std::cout << "index size: " << indices->size() << std::endl;

        // Specify vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        // clear
        vertices->clear();
        tempModelGO->indexCount = indices->size();
        indices->clear();

        modelGO = std::unique_ptr<ModelGraphicalObject>(tempModelGO);
    }

private:
    bool processLine(const std::string& line) {
        if (line.empty() || line[0] == '#') {
            return true;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) {
            std::cerr << "Invalid line: " << line << std::endl;
            return false;
        }

        if (tokens[0] == "v") {
            return processVertex(tokens);
        } else if (tokens[0] == "f") {
            return processFace(tokens);
        } else {
            std::cerr << "Invalid token: " << tokens[0] << std::endl;
            return false;
        }
    }

    bool processVertex(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4) {
            std::cerr << "Invalid vertex line: " << tokens[0] << std::endl;
            return false;
        }

        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        float z = std::stof(tokens[3]);

        vertices->push_back(x);
        vertices->push_back(y);
        vertices->push_back(z);

        return true;
    }

    bool processFace(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4) {
            std::cerr << "Invalid face line: " << tokens[0] << std::endl;
            return false;
        }

        int v1 = std::stoi(tokens[1]) - 1;
        int v2 = std::stoi(tokens[2]) - 1;
        int v3 = std::stoi(tokens[3]) - 1;

        indices->push_back(v1);
        indices->push_back(v2);
        indices->push_back(v3);

        return true;
    }
};
