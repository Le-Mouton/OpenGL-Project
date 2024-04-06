#version 330 core
out vec4 Fragcolor;

uniform vec3 coord;

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

	//float constant;

    //float linear;
    //float quadratic;

	//vec3 direction;
	//float cutOff;
};

uniform Material material;
uniform Light light;

void main()
{
	//vec3 lightDir = normalize(light.position - fragPos);
	//float theta = dot(lightDir, normalize(-light.direction));

	//if(theta > light.cutOff)
	//{
		//float distance    = length(light.position - fragPos);
		//float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		//vec3 ambient = material.diffuse * light.ambient;

		//vec3 normal = normalize(norm);
		//vec3 lightDir = normalize(light.position - fragPos);
		//float diff = max(dot(norm, lightDir), 0.0);
		//vec3 diffuse = (diff*material.diffuse) * light.diffuse;

		//vec3 viewDir = normalize(viewPos - fragPos);
		//vec3 reflectDir = reflect(-lightDir, norm);
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		//vec3 specular = light.specular * (spec * material.diffuse);

		//ambient  *= attenuation;
		//diffuse  *= attenuation;
		//specular *= attenuation;

		//vec3 result = norm;//(ambient + diffuse + specular);

        vec3 result = vec3(1.0, 1.0, 1.0);

		Fragcolor = vec4(result, 1.0);
	//}
	//else
	//	Fragcolor = vec4(light.ambient * material.diffuse, 1.0);
}