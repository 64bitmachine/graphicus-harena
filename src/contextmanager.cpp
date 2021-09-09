#include<contextmanager.h>

using namespace cg;

ContextManager::ContextManager()
{
}

ContextManager::~ContextManager()
{
}

void ContextManager::init()
{
    glfwInit();
    setOptions();

    // creating a context
    window = glfwCreateWindow(width, height, "CG App", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "failed to create glfw window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
}

void ContextManager::setOptions()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *ContextManager::getContext()
{
    return window;
}

GLint ContextManager::getHeight()
{
    return height;
}

GLint ContextManager::getWidth()
{
    return width;
}
