#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>

class Texture {
public:
    Texture(const char* path);
    ~Texture();

    void bind(GLenum textureUnit);
    void unbind();

private:
    GLuint m_texture;
};


#endif // __TEXTURE_H__