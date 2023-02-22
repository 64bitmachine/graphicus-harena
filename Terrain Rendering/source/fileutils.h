#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <GL/glew.h>
#include <iostream>

// read shader file
static const char* readShaderSource(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return NULL;
    }

    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp);  // get file size
    fseek(fp, 0, SEEK_SET); // reset file pointer

    char* buffer = new char[size + 1];
    // read file into buffer. 1 byte at a time, size bytes, from fp
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';    // null terminate buffer
    fclose(fp);

    return buffer;
}

// create shader
static GLuint createShader(GLenum shaderType, const char* shaderSource) {
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


// raw file reader
static unsigned char* readRawFile(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return NULL;
    }

    long size = 512 * 512;

    unsigned char* buffer = new unsigned char[size];
    // read file into buffer. 1 byte at a time, size bytes, from fp
    fread(buffer, 1, size, fp);
    fclose(fp);

    std::cout << "Read file: " << filename << " successfully" << std::endl;

    return buffer;
}

#endif // __FILEUTILS_H__