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

    // create program
    // Shader* shader = new Shader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader* shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader* mirrorShader = new Shader("shaders/mirror.vs", "shaders/mirror.fs");
    // Shader* texturedPlaneShader = new Shader("shaders/plane.vs", "shaders/plane.fs");
    // Shader* rippleShader = new Shader("shaders/ripple.vs", "shaders/ripple.fs");
    assert(glGetError()== GL_NO_ERROR);

    // Skybox* skybox = new Skybox();
    // GLuint checkerTexture = generateCheckerboardTexture();
    // GLuint cubemapTexture = generateCubeMapTexture();
    assert(glGetError()== GL_NO_ERROR);

    cuboid = new Cuboid(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 
    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(1.0, 0.0f, 0.0f)), false);


    // cuboid2  = new Cuboid(glm::vec3(-1.5f, -3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 
    // glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(1.0, 0.0f, 0.0f)), true);

    rectangle = new Rectangle(1.0f, 1.0f);
    rectangle->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    rectangle->setNormal(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
    assert(glGetError()== GL_NO_ERROR);

    InitFBO();
    assert(glGetError()== GL_NO_ERROR);

    // camera
    g_camera = new Camera(glm::vec3(0.0f, 2.0f, 5.0f));

    // projection and view matrix
    glm::mat4 projection;
    glm::mat4 view;

    float currentFrame;
    assert(glGetError()== GL_NO_ERROR);


    cuboid->setShader(shader);
    // cuboid->createAxes();
    // cuboid2->setShader(shader);
    // cuboid2->createAxes();
    // skybox->setShader(shader);
    // skybox->setTexture(cubemapTexture);
    rectangle->setShader(mirrorShader);

    // texturedPlane
    // TexturedPlane* texturedPlane = new TexturedPlane(10, 10);
    // texturedPlane->setShader(texturedPlaneShader);
    // texturedPlane->setTexture(checkerTexture);

    // Grid
    // Grid* grid = new Grid(10, 10);
    // grid->setShader(shader);

    Scene* scene = new Scene();
    // scene->add(texturedPlane);
    // scene->add(skybox);
    // scene->add(grid);
    scene->add(cuboid);
    // scene->add(cuboid2);
    // scene->add(rectangle);
    assert(glGetError()== GL_NO_ERROR);

    // for(int i = 0; i < 50; i++) cuboid2->rotate(2);
    // for(int i = 0; i < 20; i++) cuboid2->rotate(1);
    
    do {

        // per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // get time elapsed
        // shader->setFloat("time", currentFrame * 4.0f);

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // projection and view matrix
        projection = glm::perspective(glm::radians(g_camera->get_zoom()),
         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        view = g_camera->get_view_matrix();
        scene->render(&projection, &view);

        // ============== Mirror code =========================
        //store the current modelview matrix
        glm::mat4 MV = view * (*cuboid->modelMat);
        glm::mat4 oldMV = MV;

        //now change the view matrix to where the mirror is
        //reflect the view vector in the mirror normal direction
        glm::vec3 V = glm::vec3(-MV[2][0], -MV[2][1], -MV[2][2]);
        glm::vec3 R = glm::reflect(V, rectangle->getNormal());

        //place the virtual camera at the mirro position
        MV = glm::lookAt(rectangle->getPosition(), rectangle->getPosition() + R, glm::vec3(0,1,0));

        //since mirror image is laterally inverted, we multiply the MV matrix by (-1,1,1)
        MV = glm::scale(MV, glm::vec3(-1,1,1));

        //enable FBO 
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
        //render to colour attachment 0
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //clear the colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //show the mirror from the front side only
        if(glm::dot(V, rectangle->getNormal()) < 0) {
            cuboid->render(&projection, &view);
        } 
        
        //unbind the FBO
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        
        //restore the default back buffer 
        glDrawBuffer(GL_BACK_LEFT);

        //bind the FBO output at the current texture 
        glBindTexture(GL_TEXTURE_2D, renderTextureID);

        //render mirror
        rectangle->render(&projection, &view);
        // ====================================================

        glfwSwapBuffers(window);
        glfwPollEvents();
        // std::cout << "GL Error = " << glGetError() << std::endl;
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}