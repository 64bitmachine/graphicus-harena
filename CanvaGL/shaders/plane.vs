#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

// vertex shader output
smooth out vec2 TexCoords;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1);
    
    // textured plane
    TexCoords = aPos.xz;
}