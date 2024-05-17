#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"



void screenToWorldUsingDepthBuffer(GLFWwindow* window, int SCR_WIDTH, int SCR_HEIGHT, Camera* g_camera) {
    std::cout << "left click" << std::endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << xpos << " " << ypos << std::endl;

    float winZ = 0;
    glReadPixels(xpos, SCR_HEIGHT - ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    std::cout << "z value: " << winZ << std::endl;

    // unproject
    glm::vec3 objPt = glm::unProject(glm::vec3(xpos, SCR_HEIGHT - ypos, winZ),
        g_camera->get_view_matrix(), 
        glm::perspective(glm::radians(g_camera->get_zoom()),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f),
        glm::vec4(0, 0, SCR_WIDTH, SCR_HEIGHT));

    std::cout << "object space: " << objPt.x << " " << objPt.y << " " << objPt.z << std::endl;
}

GLuint generateCheckerboardTexture() {
    GLuint texture;
    // Checkerboard pattern size
    const int texWidth = 128;
    const int texHeight = 128;

    // Generate checkerboard pattern data
    GLubyte data[texWidth][texHeight];
    for (int j = 0; j < texWidth; ++j) {
        for (int i = 0; i < texHeight; ++i) {
            data[i][j] = (i<=64 && j<=64 || i>64 && j>64 )?255:0;
        }
    }

    // Generate texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // print gl error
    // std::cout << "GL Error = " << glGetError() << std::endl;
    assert(glGetError()== GL_NO_ERROR);

    // Set maximum anisotropy setting (optional)
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

    assert(glGetError()== GL_NO_ERROR);

    // Set mipmap base and max level
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    // Allocate texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texWidth, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    assert(glGetError()== GL_NO_ERROR);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

#endif