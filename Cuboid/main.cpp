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
bool isMouseCameraActive = false;
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

glm::vec3 screenToWorld(GLFWwindow* window, double mouseX, double mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    // // Invert mouseY to match OpenGL coordinate system
    // mouseY = screenHeight - mouseY;

    // // Convert mouse position to normalized device coordinates (NDC)
    // float x = (2.0f * mouseX) / screenWidth - 1.0f;
    // float y = (2.0f * mouseY) / screenHeight - 1.0f;

    // // Construct clip space coordinates
    // glm::vec4 clipSpacePos(x, y, -1.0f, 1.0f);

    // // Invert projection matrix
    // glm::mat4 invertedProjectionMatrix = glm::inverse(projectionMatrix);

    // // Obtain eye space coordinates
    // glm::vec4 eyeSpacePos = invertedProjectionMatrix * clipSpacePos;
    // eyeSpacePos.z = -1.0f;
    // eyeSpacePos.w = 0.0f;

    // // Invert view matrix
    // glm::mat4 invertedViewMatrix = glm::inverse(viewMatrix);

    // // Obtain world space coordinates
    // glm::vec4 worldSpacePos = invertedViewMatrix * eyeSpacePos;
    // worldSpacePos /= worldSpacePos.w;

    // return glm::vec3(worldSpacePos);


    // ================

    float x,y,z;
    x = float((mouseX / SCR_WIDTH) * 2 - 1);
    y = float(1 - (mouseY / SCR_HEIGHT) * 2);
    printf("mouse click pos: %f, %f\n", x, y);
    glReadPixels(mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

    printf("z value: %f\n", z);

    glm::mat4 inv_projection = glm::inverse(projectionMatrix * viewMatrix);

    glm::vec4 pr = inv_projection*glm::vec4(x,y,-z,1.0f);
    pr = pr/pr.w;
    glm::vec4 pr1 = inv_projection*glm::vec4(x,y,z,1.0f);
    pr1 = pr1/pr1.w;
    // cout << glm::to_string(pr) << endl;
    // cout << glm::to_string(pr1) << endl;
    pr = pr + glm::normalize(pr1-pr);
    // glm::vec4 pos = inv_view*pr;
    
    return glm::vec3(pr.x, pr.y, pr.z);
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

    // Toggle mouse camera mode using C
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        isMouseCameraActive = !isMouseCameraActive;

        printf("Mouse camera mode: %s\n", isMouseCameraActive ? "Active" : "Inactive");
    }
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

    if (isMouseCameraActive) {
        g_camera->process_mouse_movement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera->process_mouse_scroll(yoffset);
}

double clickX = 0.0, clickReleaseX = 0.0;
double clickY = 0.0, clickReleaseY = 0.0;
bool isMouseClicked = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        isMouseClicked = true;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // print cursor position
        std::cout << "Cursor position: " << mouseX << ", " << mouseY << std::endl;

        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        // Example view and projection matrices (you need to replace these with your actual matrices)
        glm::mat4 viewMatrix = g_camera->get_view_matrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(g_camera->get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::vec3 worldPos = screenToWorld(window, mouseX, mouseY, screenWidth, screenHeight, viewMatrix, projectionMatrix);

        // Output the corresponding 3D position in the scene
        printf("World Position: (%f, %f, %f)\n", worldPos.x, worldPos.y, worldPos.z);
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

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// setup opengl options
	glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    cuboid->setShader(shader);

    do {

        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // projection and view matrix
        projection = glm::perspective(glm::radians(g_camera->get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = g_camera->get_view_matrix();

        // set projection and view matrix
        // shader->setMat4("projection", projection);
        // shader->setMat4("view", view);

        cuboid->setProjMat(&projection);
        cuboid->setViewMat(&view);

        cuboid->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}