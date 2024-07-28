#include <gtest/gtest.h>
#include "cuboid.h"
#include "2d/Rectangle.h"
#include "utils.h"

class CuboidPlaneTest : public ::testing::Test {
protected:
    Cuboid* cuboid;
    Rectangle* rectangle;
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
        // delete cuboid;
        // delete rectangle;
        glfwTerminate();
    }
};

// Test case to verify the center of the cuboid
TEST_F(CuboidPlaneTest, CenterTest) {
    // Initialize the parameters for the Cuboid
    glm::vec3 expectedCenter(1.0f, 1.0f, 1.0f);
    glm::vec3 dimensions(2.0f, 2.0f, 2.0f); // length, breadth, height
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    glm::vec3 rightVector(1.0f, 0.0f, 0.0f);

    // Create a Cuboid object
    cuboid = new Cuboid(expectedCenter, dimensions, upVector, rightVector);

    // Get the center of the cuboid
    glm::vec3 actualCenter = cuboid->getCenter();

    // Verify that the center is as expected
    EXPECT_EQ(actualCenter, expectedCenter);
}

TEST_F(CuboidPlaneTest, Initialization) {
    float width = 4.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);
    // std::cout << rectangle->getNormal().x << " " << rectangle->getNormal().y << " " << rectangle->getNormal().z << std::endl;
    // std::cout << rectangle->getPosition().x << " " << rectangle->getPosition().y << " " << rectangle->getPosition().z << std::endl;
    EXPECT_EQ(rectangle->getNormal(), glm::vec3(0.0f, 0.0f, -1.0f)) << "Normal vector should be (0.0, 0.0, -1.0)";
    EXPECT_EQ(rectangle->getPosition(), glm::vec3(0.0f, 0.0f, 0.0f)) << "Initial position should be (0.0, 0.0, 0.0)";
}

TEST_F(CuboidPlaneTest, SetPosition) {
    float width = 4.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);

    glm::vec3 newPosition(1.0f, 2.0f, 3.0f);
    rectangle->setPosition(newPosition);

    EXPECT_EQ(rectangle->getPosition(), newPosition) << "Position should be (1.0, 2.0, 3.0)";
}

TEST_F(CuboidPlaneTest, SetNormal) {
    float width = 4.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);

    glm::vec3 newNormal(1.0f, 0.0f, 0.0f);
    rectangle->setNormal(newNormal);

    EXPECT_EQ(rectangle->getNormal(), glm::normalize(newNormal)) << "Normal should be (1.0, 0.0, 0.0)";
}

TEST_F(CuboidPlaneTest, SimpleIntersection) {
    
    // Initialize the parameters for the Cuboid
    glm::vec3 expectedCenter(1.0f, 1.0f, 1.0f);
    glm::vec3 dimensions(2.0f, 2.0f, 2.0f); // length, breadth, height
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    glm::vec3 rightVector(1.0f, 0.0f, 0.0f);

    // Create a Cuboid object
    cuboid = new Cuboid(expectedCenter, dimensions, upVector, rightVector);

    float width = 4.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);

    // Get the intersection points
    std::vector<glm::vec3> intersectionPoints = cuboid->cuboidPlaneIntersection(*rectangle);

    // Check that there are intersection points
    ASSERT_GT(intersectionPoints.size(), 0);
}

TEST_F(CuboidPlaneTest, NoIntersection) {
    // Initialize the parameters for the Cuboid
    glm::vec3 expectedCenter(1.0f, 1.0f, 1.0f);
    glm::vec3 dimensions(2.0f, 2.0f, 2.0f); // length, breadth, height
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    glm::vec3 rightVector(1.0f, 0.0f, 0.0f);

    // Create a Cuboid object
    cuboid = new Cuboid(expectedCenter, dimensions, upVector, rightVector);

    float width = 4.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);

    // Set the rectangle's position to not intersect with the cuboid
    rectangle->setPosition(glm::vec3(6, 6, 6));

    // Get the intersection points
    std::vector<glm::vec3> intersectionPoints = cuboid->cuboidPlaneIntersection(*rectangle);

    // Check that there are no intersection points
    ASSERT_EQ(intersectionPoints.size(), 0);
}

TEST_F(CuboidPlaneTest, ExplicitIntersection) {
    // Initialize the parameters for the Cuboid
    glm::vec3 expectedCenter(0.0f, 0.0f, 0.0f);
    glm::vec3 dimensions(2.0f, 2.0f, 2.0f); // length, breadth, height
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    glm::vec3 rightVector(1.0f, 0.0f, 0.0f);

    // Create a Cuboid object
    cuboid = new Cuboid(expectedCenter, dimensions, upVector, rightVector);

    float width = 2.0f;
    float height = 2.0f;
    rectangle = new Rectangle(width, height);

    // Set the rectangle's position and normal to intersect with the cuboid
    rectangle->setPosition(glm::vec3(0, 0, 0));
    rectangle->setNormal(glm::vec3(0, 0, 1));

    // Get the intersection points
    std::vector<glm::vec3> intersectionPoints = cuboid->cuboidPlaneIntersection(*rectangle);

    // Check that there are intersection points
    ASSERT_EQ(intersectionPoints.size(), 4);

    // Check the intersection points
    std::vector<glm::vec3> expectedPoints = {
        glm::vec3(1, 1, 0),
        glm::vec3(1, -1, 0),
        glm::vec3(-1, 1, 0),
        glm::vec3(-1, -1, 0)
    };

    for (const auto& point : intersectionPoints) {
        bool found = false;
        for (const auto& expectedPoint : expectedPoints) {
            if (point == expectedPoint) {
                found = true;
                break;
            }
        }
        ASSERT_TRUE(found);
    }
}

