#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;

uniform float gMinHeight;
uniform float gMaxHeight;
out vec3 color;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    float dh = gMaxHeight - gMinHeight;
    float height_ratio = (aPos.y - gMinHeight) / dh;
    float c = 0.8 * height_ratio + 0.2;
    color = vec3(c);
}