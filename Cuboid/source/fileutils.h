#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <GL/glew.h>
#include <iostream>

// read shader file
const char* readShaderSource(const char* filename) {
    FILE* fp;
    errno_t err;

    #ifdef _WIN32
        // Use fopen_s for Windows
        err = fopen_s(&fp, filename, "r");
        if (err != 0) {
            std::cout << "Failed to open file: " << filename << std::endl;
            return nullptr;
        }
    #else
        // Use fopen for other operating systems
        fp = fopen(filename, "r");
        if (fp == NULL) {
            std::cout << "Failed to open file: " << filename << std::endl;
            return nullptr;
        }
    #endif

    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp);  // get file size
    fseek(fp, 0, SEEK_SET); // reset file pointer

    char* buffer = new char[size + 1];
    fread(buffer, 1, size, fp); // read file into buffer
    buffer[size] = '\0';        // null terminate buffer
    fclose(fp);

    return buffer;
}


// create shader
GLuint createShader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        std::cout << "Failed to compile shader" << std::endl;
        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetShaderInfoLog(shader, logSize, NULL, logMsg);
        std::cout << logMsg << std::endl;
        delete[] logMsg;
        return 0;
    }

    return shader;
}


#endif // __FILEUTILS_H__