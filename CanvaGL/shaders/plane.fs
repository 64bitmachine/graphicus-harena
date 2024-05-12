#version 330 core
out vec4 FragColor;

// input from the vertex shader and interpolated 2D texture coordinates
smooth in vec2 TexCoords;

uniform sampler2D textureMap;

void main() {
    FragColor = texture(textureMap, TexCoords).rrrr;
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}