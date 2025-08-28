#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;

flat out vec3 vNormal; // Pass normal to fragment shader (flat shading)

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    vNormal = aNormal;
}