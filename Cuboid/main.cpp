#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include "source/camera.h"
#include "source/shader.h"
#include "source/cuboid.h"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool  firstMouse = true;
Camera* g_camera;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

enum ScaleDimension { LENGTH_BREADTH = 0, LENGTH_HEIGHT = 1, BREADTH_HEIGHT = 2 };

ScaleDimension getScaleDimension(glm::vec2 mouseDragVec, glm::vec3 cuboidUpVec, glm::vec3 cuboidRightVec) {
    // Project the mouse drag vector onto the plane defined by the cuboid's up and right vectors
    glm::vec2 projVec;
    projVec.x = glm::dot(mouseDragVec, glm::vec2(cuboidRightVec.x, cuboidRightVec.y));
    projVec.y = glm::dot(mouseDragVec, glm::vec2(cuboidUpVec.x, cuboidUpVec.y));

    // Calculate the magnitudes of the projected vectors
    float projXMag = glm::length(projVec.x);
    float projYMag = glm::length(projVec.y);

    // Determine which dimensions to scale based on the magnitudes of the projected vectors
    if (projXMag > projYMag) {
        return LENGTH_BREADTH;
    } else {
        return LENGTH_HEIGHT;
    }
    // If you want to consider BREADTH_HEIGHT as well, you can compare the magnitudes of the other two projected vectors.
}

void initialiseBuffer(GLuint* vbo, GLuint* vao) {
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera->process_keyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera->process_keyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera->process_keyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera->process_keyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec2 getRelVec(int mouseX, int mouseY, int initialClickX, int initialClickY, int width, int height) {
    // Convert coordinates to NDC (-1 to 1)
    float mouseX_NDC = (2.0f * mouseX) / width - 1.0f;
    float mouseY_NDC = 1.0f - (2.0f * mouseY) / height;

    float initialClickX_NDC = (2.0f * initialClickX) / width - 1.0f;
    float initialClickY_NDC = 1.0f - (2.0f * initialClickY) / height;

    // Calculate relative vector
    glm::vec2 relativeVector = glm::vec2(mouseX_NDC - initialClickX_NDC, mouseY_NDC - initialClickY_NDC);

    return relativeVector;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    g_camera->process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera->process_mouse_scroll(yoffset);
}

double clickX = 0.0, clickReleaseX = 0.0;
double clickY = 0.0, clickReleaseY = 0.0;
bool isMouseClicked = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Get current mouse position
        glfwGetCursorPos(window, &clickX, &clickY);
        isMouseClicked = true;

        // print cursor position
        std::cout << "Cursor position: " << clickX << ", " << clickY << std::endl;
    } if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isMouseClicked = false;
        
        glfwGetCursorPos(window, &clickReleaseX, &clickReleaseY);
        // print cursor position
        std::cout << "Cursor position: " << clickReleaseX << ", " << clickReleaseY << std::endl;

        // Calculate relative vector
        glm::vec2 relativeVec = getRelVec(clickReleaseX, clickReleaseY, clickX, clickY, SCR_WIDTH, SCR_HEIGHT);

        std::cout << "Relative vector: " << relativeVec.x << ", " << relativeVec.y << std::endl;
    }
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // mac os x
    #ifdef __UNIX__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #elif __APPLE__
            // Define version and compatibility settings
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // create program
    Shader* shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    // initialise buffer
    // GLuint vbo, vao;
    // initialiseBuffer(&vbo, &vao);

    // create cuboid
    Cuboid* cuboid = new Cuboid(glm::vec3(0.0f), 2.0f, 1.0f, 1.5f, 
    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // camera
    g_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

    // projection and view matrix
    glm::mat4 projection;
    glm::mat4 view;

    float currentFrame;

    do {

        // use program
        shader->use();

        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // projection and view matrix
        projection = glm::perspective(glm::radians(g_camera->get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = g_camera->get_view_matrix();

        // set projection and view matrix
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        cuboid->render(projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}