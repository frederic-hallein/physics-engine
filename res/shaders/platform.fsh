#version 330 core
flat in vec3 vNormal;

out vec4 FragColor;

uniform vec3 lightDir;

void main()
{
    //vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    //float diff = max(dot(normalize(vNormal), lightDir), 0.0);
    float diff = max(dot(normalize(vNormal), normalize(lightDir)), 0.0);
    vec3 baseColor = vec3(0.7, 0.7, 0.7);
    FragColor = vec4(baseColor * diff, 1.0);
}