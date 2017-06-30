#version 330 core

in vec3 CubeColor;

out vec4 FragColor;

void main()
{    
    FragColor = vec4(CubeColor, 1.0f);
}
