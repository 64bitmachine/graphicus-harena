#include "texture.h"
#include "../thirdparty/stb_image.h"
#include <iostream>

Texture::Texture(const char *path) {

    // load and generate the texture
    stbi_set_flip_vertically_on_load(true); // because OpenGL expects the origin to be
                                            // in the bottom left corner of the image
    
    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
                                // zero means that we want to load the image with the
                                // number of channels it was saved with (3 or 4)

    if (data) {
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unbind();

        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        // reason using stbi_failure_reason() is because stbi_load() returns a null
        std::cout << stbi_failure_reason() << std::endl;
    }

}

Texture::~Texture(){}

void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}