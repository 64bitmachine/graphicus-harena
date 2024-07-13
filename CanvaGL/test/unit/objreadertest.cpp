#include <gtest/gtest.h>
#include "meshreader/modelreaderfactory.h"
#include "utils.h"

class ObjReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
        initCanvaGL();

        GLFWwindow* window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to open GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to initialize GLEW" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void TearDown() override {
        // Teardown code if needed
    }
};

TEST_F(ObjReaderTest, TestReadValidFile) {
    std::string validFilePath = "resources/valid.obj";
    std::unique_ptr<ModelReader> reader = ModelReaderFactory::createModelReader(validFilePath);
    ASSERT_NE(nullptr, reader->loadModel());
}

TEST_F(ObjReaderTest, TestReadInvalidFile) {
    std::string invalidFilePath = "resources/invalid.obj";
    std::unique_ptr<ModelReader> reader = ModelReaderFactory::createModelReader(invalidFilePath);
    ASSERT_EQ(nullptr, reader->loadModel());
}

TEST_F(ObjReaderTest, TestReadEmptyFile) {
    std::string emptyFilePath = "resources/empty.obj";
    std::unique_ptr<ModelReader> reader = ModelReaderFactory::createModelReader(emptyFilePath);
    ASSERT_NE(nullptr, reader->loadModel());
}

TEST_F(ObjReaderTest, TestReadTeapot) {
    std::string teapotFilePath = "resources/teapot.obj";
    std::unique_ptr<ModelReader> reader = ModelReaderFactory::createModelReader(teapotFilePath);
    ASSERT_NE(nullptr, reader->loadModel());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
