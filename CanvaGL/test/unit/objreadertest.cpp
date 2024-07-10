#include <gtest/gtest.h>
#include "meshreader/objreader.h"

class ObjReaderTest : public ::testing::Test {
protected:
    OBJModelReader reader;

    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Teardown code if needed
    }
};

TEST_F(ObjReaderTest, TestReadValidFile) {
    std::string validFilePath = "resources/valid.obj";
    ASSERT_TRUE(reader.loadModel(validFilePath));
}

TEST_F(ObjReaderTest, TestReadInvalidFile) {
    std::string invalidFilePath = "resources/invalid.obj";
    ASSERT_FALSE(reader.loadModel(invalidFilePath));
}

TEST_F(ObjReaderTest, TestReadEmptyFile) {
    std::string emptyFilePath = "resources/empty.obj";
    ASSERT_TRUE(reader.loadModel(emptyFilePath));
}

TEST_F(ObjReaderTest, TestReadTeapot) {
    std::string teapotFilePath = "resources/teapot.obj";
    ASSERT_TRUE(reader.loadModel(teapotFilePath));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
