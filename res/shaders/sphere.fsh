#version 330 core
out vec4 FragColor;

void main()
{
    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    FragColor = vec4(baseColor, 1.0);
}