#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <GL/glew.h>
#include <iostream>
#include <SOIL.h>
#include <filesystem>
#include <vector>
#include <cstring>

// read shader file
static const char* readShaderSource(const char* filename) {
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


static GLubyte* readImage(const char* filename, int& width, int& height, int& channels) {
    GLubyte* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_AUTO);
    return image;
}

static std::vector<std::string> getFilenames(const char* foldername) {
    std::vector<std::string> filenames;
    std::filesystem::path folderPath(foldername);

    try {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string filenameStr = entry.path().filename().string();
                // end with .jpg
                if (filenameStr.find(".jpg") == std::string::npos) continue;
                // foldername + filename
                std::string fullFilename = std::string(foldername) + "/" + filenameStr;
                filenames.push_back(fullFilename);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // Handle errors here
    }

    return filenames;
}

#endif // __FILEUTILS_H__