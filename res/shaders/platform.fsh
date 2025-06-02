#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//
//void main()
//{
//    FragColor = vec4(lightColor * objectColor, 1.0);
//}