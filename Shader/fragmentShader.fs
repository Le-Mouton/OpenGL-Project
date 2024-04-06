#version 330 core

out vec4 Fragcolor;

uniform vec3 viewPos;

in vec3 norm;
in vec3 fragPos;

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
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(norm);

    for(int i = 0; i < activeLights; i++) {
        vec3 lightDir = normalize(light[i].position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = (diff * material.diffuse) * light[i].diffuse;

        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light[i].specular * (spec * material.specular);

        vec3 ambient = material.diffuse * light[i].ambient;

        result += (ambient + diffuse + specular);
    }

    Fragcolor = vec4(result, 1.0);
}
