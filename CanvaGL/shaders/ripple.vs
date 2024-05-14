#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform float time;

const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main() {
    float distance = length(aPos);
    float y = amplitude * sin(- PI * frequency * distance + time);
    gl_Position = projection * view * model * vec4(aPos.x, y, aPos.z, 1);
}