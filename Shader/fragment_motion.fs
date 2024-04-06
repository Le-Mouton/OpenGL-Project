#version 330 core

out vec4 Fragcolor;

uniform vec3 coord;
uniform vec3 lightFlameSource;

in vec3 norm;
in vec3 fragPos;
in float deltaTime;

struct Material{
    vec3 diffuse;
    vec3 specular;
	float shininess;
};

struct Light{
	vec3 specular;
	vec3 ambient;
	vec3 diffuse;

	vec3 position;
};

#define MAX_LIGHTS 50

uniform int activeLights;
uniform Material material;
uniform Light light[MAX_LIGHTS];

void main()
{
    vec3 result = vec3(0.0,0.0,0.0);
    for(int i =0; i< activeLights; i++){
        result += material.diffuse*1/distance(fragPos, light[i].position) * (cos(deltaTime*0.25) + 2);
    }
    Fragcolor = vec4(result, 1);
}