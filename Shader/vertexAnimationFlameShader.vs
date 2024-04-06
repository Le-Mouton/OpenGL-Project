#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float deltatime;

out vec3 norm;
out vec3 fragPos;
out float deltaTime;

void main()
{
   deltaTime = deltatime;
   fragPos = vec3(model * vec4(aPos, 1.0));
   fragPos = vec3(fragPos.x*cos(deltatime + fragPos.y/10), fragPos.y*sin(deltatime + fragPos.x/10), fragPos.z*sin(deltatime + fragPos.y/10));

   norm = mat3(transpose(inverse(model))) * aNorm;
   //norm = vec3(norm.x*cos(deltatime + y/10), norm.y*cos(deltatime + y/10), norm.z*cos(deltatime + y/10));
   gl_Position = projection * view * vec4(fragPos, 1.0);
}