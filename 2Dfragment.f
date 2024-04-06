#version 330 core
out vec4 Color;

uniform vec3 color;

void main()
{    
    Color = vec4(color, 1.0);
}  