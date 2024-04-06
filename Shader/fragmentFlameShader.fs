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

uniform Material material;
uniform Light light;

void main()
{
        vec3 result = material.diffuse*1/distance(fragPos, light.position) * (3 + cos(0.5*deltaTime));
		Fragcolor = vec4(result, 1);
}