#version 330 core

layout(location=0) out vec4 FragColor;

uniform samplerCube cubeMap;

smooth in vec3 uv;

void main() {
    FragColor = texture(cubeMap, uv);
}