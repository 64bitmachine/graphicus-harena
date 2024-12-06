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
#include "source/scene.h"
#include "source/texturedplane.h"
#include "source/grid.h"
#include "source/utils.h"
#include "source/skybox.h"
#include "source/2d/Rectangle.h"
#include "meshreader/modelreader.h"
#include "meshreader/modelreaderfactory.h"
#include "source/cloth/particle.h"
#include "source/cloth/constraint.h"

float rippleTime = 0.0f;
//ripple displacement speed
// const float SPEED = 2;

Rectangle* rectangle = nullptr;
Cuboid *cuboid = nullptr;
// Cuboid *cuboid2 = nullptr;

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
bool test = false;


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

//FBO and render buffer object ID
GLuint fboID, rbID;

//offscreen render texture ID
GLuint renderTextureID;

//initialize FBO
void InitFBO() {
	//generate and bind fbo ID
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);

	//generate and bind render buffer ID
	glGenRenderbuffers(1, &rbID);
	glBindRenderbuffer(GL_RENDERBUFFER, rbID);

	//set the render buffer storage
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, SCR_WIDTH, SCR_HEIGHT);

	//generate the offscreen texture
	glGenTextures(1, &renderTextureID);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	//bind the renderTextureID as colour attachment of FBO
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTextureID, 0);
	//set the render buffer as the depth attachment of FBO
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, rbID);

	//check for frame buffer completeness status
	GLuint status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	if(status==GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO setup succeeded.\n");
	} else {
		printf("Error in FBO setup.\n");
	}

	//unbind the texture and FBO
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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

    // if (test == false && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    //     test = true;
    //     cuboid2->getRelativeOrientation(cuboid);
    // }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        test = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // mouse left click
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        screenToWorldUsingDepthBuffer(window, SCR_WIDTH, SCR_HEIGHT, g_camera);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        std::cout << "left click release" << std::endl;
    }
}


const int ROW = 10;
const int COL = 10;
const float REST_DISTANCE = 0.5f;
const float GRAVITY = -0.05f;

int main(int argc, char** argv) {

    initCanvaGL();

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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

	// setup opengl options
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    rectangle = new Rectangle(1.0f, 1.0f);
    rectangle->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    rectangle->setNormal(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
    assert(glGetError()== GL_NO_ERROR);

    // create program
    Shader* shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    assert(glGetError()== GL_NO_ERROR);

    std::vector<Particle*> particles;
    std::vector<Constraint> constraints;

    // particles.emplace_back(0.0f, 0.0f, -2.0f, false);
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            float x = -2.0f + col * REST_DISTANCE;
            float y = row * REST_DISTANCE;
            bool pinned = (row == ROW-1);
            // std::cout << "x " << x << " y " << y << std::endl;
            particles.push_back(new Particle(x, y, -2.0f, pinned));
        }
    }

    // Initialize constraints
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (col < COL - 1) {
                // Horizontal constraint
                constraints.emplace_back(particles[row * COL + col], particles[row * COL + col + 1]);
            }
            if (row < ROW - 1) {
                // Vertical constraint
                constraints.emplace_back(particles[row * COL + col], particles[(row + 1) * COL + col]);
            }
        }
    }

    assert(glGetError()== GL_NO_ERROR);

    // InitFBO();
    assert(glGetError()== GL_NO_ERROR);

    // camera
    g_camera = new Camera(glm::vec3(0.0f, 2.0f, 10.0f));

    // projection and view matrix
    glm::mat4 projection;
    glm::mat4 view;

    float currentFrame;
    assert(glGetError()== GL_NO_ERROR);

    Scene* scene = new Scene();
    for(auto& particle: particles) {
        particle->setShader(shader);
        scene->add(particle);
    }
    rectangle->setShader(shader);
    // scene->add(rectangle);
    assert(glGetError()== GL_NO_ERROR);

    do {

        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //apply gravity and update particles
        for (auto& particle : particles) {
            particle->apply_force(glm::vec3(0, GRAVITY, 0));
            particle->update(deltaTime);
            particle->constrain_to_bounds(0, 0);
        }

        // get time elapsed
        // shader->setFloat("time", currentFrame * 4.0f);

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // projection and view matrix
        projection = glm::perspective(glm::radians(g_camera->get_zoom()),
         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        view = g_camera->get_view_matrix();
        assert(glGetError()== GL_NO_ERROR);
        scene->render(&projection, &view);

        assert(glGetError()== GL_NO_ERROR);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // std::cout << "GL Error = " << glGetError() << std::endl;
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}