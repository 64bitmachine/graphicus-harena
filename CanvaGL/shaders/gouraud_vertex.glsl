#version 330 core
layout (location = 0) in vec3 aVertex;
layout (location = 0) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 light_position;
uniform vec3 eye_position;
smooth out vec4 color;

void main() {
    
    vec4 lightPosInEyeSpace = view * vec4(light_position,1);
    vec4 vertexPosInEyeSpace = view * model * vec4(aVertex, 1);

    vec3 normalEyeSpace = normalize(transpose(inverse(mat3(view * model))) * aNormal);

    gl_Position = projection * view * model * vec4(aVertex, 1);
}