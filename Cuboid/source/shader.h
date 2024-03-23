#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{

public:
    unsigned int m_ID;

    // constructor generates the shader on the fly
    Shader(const char *vertexPath, const char *fragmentPath);

    // activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;

private:
    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // __SHADER_H__