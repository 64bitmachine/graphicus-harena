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

Cuboid* cuboid = nullptr;

double mouseX = 0.0, clickReleaseX = 0.0;
double mouseY = 0.0, clickReleaseY = 0.0;
bool isMouseClicked = false;
bool keyReset = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool  firstMouse = true;
bool isMouseCameraActive = false;
bool rescaleCuboidUsingCornerPoint = false;
bool cuboidMoveMode = false;
Camera* g_camera;

// initialMouse click marker
Cuboid* initialCuboid = nullptr;
glm::vec3 initialCuboidDim = glm::vec3(0.01f, 0.01f, 0.01f);

Cuboid* cornerPoint = nullptr;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

enum ScaleDimension { 
    LENGTH_BREADTH = 0,
    LENGTH_HEIGHT = 1, 
    BREADTH_HEIGHT = 2,
    LENGTH = 3,
    BREADTH = 4,
    HEIGHT = 5,
    INVALID = 6 };

ScaleDimension getScaleDimension(glm::vec3 mouseDragVec, glm::vec3 cuboidUpVec, glm::vec3 cuboidRightVec) {
    // Calculate normal vector to the plane defined by cuboidUpVec and cuboidRightVec
    glm::vec3 normal = glm::cross(cuboidUpVec, cuboidRightVec);

    // Project mouseDragVec onto the planes defined by cuboidUpVec, cuboidRightVec, and normal
    float projX = glm::dot(mouseDragVec, cuboidRightVec);
    float projY = glm::dot(mouseDragVec, cuboidUpVec);
    float projZ = glm::dot(mouseDragVec, normal);

    // std::cout << "projX: " << projX << std::endl;
    // std::cout << "projY: " << projY << std::endl;
    // std::cout << "projZ: " << projZ << std::endl;

    // Take absolute values for projection components
    float absProjX = std::abs(projX);
    float absProjY = std::abs(projY);
    float absProjZ = std::abs(projZ);

    // std::cout << "absProjX: " << absProjX << std::endl;
    // std::cout << "absProjY: " << absProjY << std::endl;
    // std::cout << "absProjZ: " << absProjZ << std::endl;

    // Determine which dimension has the greatest projection magnitude
    if (absProjX > absProjZ && absProjY > absProjZ && absProjX && absProjY) {
        // If projection along X axis is greatest, scale along length and breadth
        return BREADTH_HEIGHT;
    } else if (absProjX > absProjY && absProjZ > absProjY && absProjX && absProjZ) {
        // If projection along Y axis is greatest, scale along length and height
        return LENGTH_BREADTH;
    } else if (absProjY > absProjX && absProjZ > absProjX && absProjY && absProjZ) {
        // If projection along Z axis is greatest, scale along breadth and height
        return LENGTH_HEIGHT;
    } if (absProjX > 0) {
        return BREADTH;
    } if (absProjY > 0) {
        return HEIGHT;
    } if (absProjZ > 0) {
        return LENGTH;
    }
    return INVALID;
}

glm::vec3 screenToWorld(GLFWwindow* window, double mouseX, double mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    float x,y,z;
    x = float((mouseX / SCR_WIDTH) * 2 - 1);
    y = float(1 - (mouseY / SCR_HEIGHT) * 2);
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

void resizeCuboid(glm::vec3 relativeVec, bool override) {
    // switch (getScaleDimension(relativeVec, cuboid->getUpVector(), cuboid->getRightVector()))
    // {
    //     case ScaleDimension::LENGTH_BREADTH:
    //         std::cout << "Length and breadth" << std::endl;
    //         break;
        
    //     case ScaleDimension::LENGTH_HEIGHT:
    //         std::cout << "Length and height" << std::endl;
    //         break;

    //     case ScaleDimension::BREADTH_HEIGHT:
    //         std::cout << "Breadth and height" << std::endl;
    //         break;

    //     case ScaleDimension::LENGTH:
    //         std::cout << "Length" << std::endl;
    //         break;
        
    //     case ScaleDimension::HEIGHT:
    //         std::cout << "Height" << std::endl;
    //         break;

    //     case ScaleDimension::BREADTH:
    //         std::cout << "Breadth" << std::endl;
    //         break;

    //     default:
    //         // print invalid scaling
    //         std::cout << "Invalid scaling" << std::endl;
    //         break;
    // }

    // if (!rescaleCuboidUsingCornerPoint) cuboid->rescale(relativeVec, override);
    if (cuboidMoveMode) cuboid->move(relativeVec, override);
    else cuboid->rescaleUsingCorner(relativeVec, override);

    // cuboid->rescale(relativeVec, override);

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

    // 1 to rotate along cuboid upVec
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        cuboid->rotate(0);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        cuboid->rotate(1);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        cuboid->rotate(2);
    }
    // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    //     rescaleCuboidUsingCornerPoint = !rescaleCuboidUsingCornerPoint;
    //     printf("Rescale cuboid using corner point: %s\n", rescaleCuboidUsingCornerPoint ? "Active" : "Inactive");
    // }

    // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !keyReset) {
    //     cuboid->rescale(glm::vec3(1.0f, 1.0f, 1.0f), true);
    //     keyReset = true;
    // }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !keyReset) {
        cuboid->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        keyReset = false;
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !keyReset) {
        // cuboid->printVerticesUsingMatrix();
        cuboid->rescale(glm::vec3(0.2f, 0.5f, 0.5f), true);
        keyReset = true;
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !keyReset) {
        cuboidMoveMode = !cuboidMoveMode;
        keyReset = true;
        printf("Cuboid move mode: %s\n", cuboidMoveMode ? "Active" : "Inactive");
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        cuboid->rescale(glm::vec3(0.01f, 0.0f, 0.0f), true);
    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        cuboid->rescale(glm::vec3(-0.01f, 0.0f, 0.0f), true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec3 getRelVec(int mouseX, int mouseY, int initialClickX, int initialClickY, int width, int height) {
    // Convert coordinates to NDC (-1 to 1)
    float mouseX_NDC = (2.0f * mouseX) / width - 1.0f;
    float mouseY_NDC = 1.0f - (2.0f * mouseY) / height;

    float initialClickX_NDC = (2.0f * initialClickX) / width - 1.0f;
    float initialClickY_NDC = 1.0f - (2.0f * initialClickY) / height;

    // Calculate relative vector
    glm::vec2 relativeVector = glm::vec2(mouseX_NDC - initialClickX_NDC, mouseY_NDC - initialClickY_NDC);

    return glm::vec3(relativeVector, 0.0f);
}

void cuboidResizeCallback(GLFWwindow* window, bool override) {
    glfwGetCursorPos(window, &clickReleaseX, &clickReleaseY);
    // Calculate relative vector
    glm::vec3 relativeVec = getRelVec(clickReleaseX, clickReleaseY, mouseX, mouseY, SCR_WIDTH, SCR_HEIGHT);
    // std::cout << "Relative vector: " << relativeVec.x << ", " << relativeVec.y << ", " << relativeVec.z << std::endl;

    float db = glm::dot(relativeVec, cuboid->getRightVector());
    float dh = glm::dot(relativeVec, cuboid->getUpVector());
    float dl = glm::dot(relativeVec, glm::cross(cuboid->getRightVector(), cuboid->getUpVector()));

    // resizeCuboid
    resizeCuboid(glm::vec3(db, dh, dl), override);
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

    if (isMouseClicked) {
        cuboidResizeCallback(window, false);
    }

    // std::cout << "xpos: " << xpos << ", ypos: " << ypos << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera->process_mouse_scroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        isMouseClicked = true;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        // Example view and projection matrices (you need to replace these with your actual matrices)
        glm::mat4 viewMatrix = g_camera->get_view_matrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(g_camera->get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);

        glm::vec3 worldPos = screenToWorld(window, mouseX, mouseY, screenWidth, screenHeight, viewMatrix, projectionMatrix);

        // Output the corresponding 3D position in the scene
        // printf("World Position: (%f, %f, %f)\n", worldPos.x, worldPos.y, worldPos.z);

        initialCuboid = new Cuboid(worldPos, initialCuboidDim, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    } if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isMouseClicked = false;
        
        cuboidResizeCallback(window, true);

        // delete cuboid
        delete initialCuboid;
        initialCuboid = nullptr;

        // std::cout << "clickReleaseX: " << clickReleaseX << ", clickReleaseY: " << clickReleaseY << std::endl;
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

	// setup opengl options
	glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // create program
    Shader* shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader* markerShader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    cuboid = new Cuboid(glm::vec3(0.0f), glm::vec3(1.0f, 1.5f, 2.0f), 
    // glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(0.95882f, 0.0f, -0.284015f)), true);
    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(1.0, 0.0f, 0.0f)), true);

    // create marker
    cornerPoint = new Cuboid(glm::vec3(-0.5, -0.75, 1), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // camera
    g_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

    // projection and view matrix
    glm::mat4 projection;
    glm::mat4 view;

    float currentFrame;

    cuboid->setShader(shader);
    cornerPoint->setShader(shader);
    cuboid->createAxes();

    do {

        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // projection and view matrix
        projection = glm::perspective(glm::radians(g_camera->get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        view = g_camera->get_view_matrix();

        // set projection and view matrix
        // shader->setMat4("projection", projection);
        // shader->setMat4("view", view);

        cuboid->setProjMat(&projection);
        cuboid->setViewMat(&view);
        cuboid->render();

        cornerPoint->setProjMat(&projection);
        cornerPoint->setViewMat(&view);
        cornerPoint->render();

        if (initialCuboid != nullptr) {
            initialCuboid->setShader(markerShader);
            initialCuboid->setProjMat(&projection);
            initialCuboid->setViewMat(&view);
            initialCuboid->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}