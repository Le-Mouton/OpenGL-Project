#include "Light.h"


Light::Light(float alpha, float beta, vec3 lightColor, vec3 lightSpecular, vec3 coord) {
    this->alpha=alpha;
    this->beta=beta;
    this->lightColor=lightColor;
    this->lightSpecular=lightSpecular;
    this->coord=coord;
}

void AddLight(std::vector<Light> list_light, Shader &shader) {
    int nbrLight = list_light.size();
    shader.setInt("activeLights", nbrLight);

    for(int i = 0; i < nbrLight; i++) {
        glm::vec3 diffuseColor = list_light[i].lightColor * glm::vec3(list_light[i].alpha);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(list_light[i].beta);

        std::ostringstream position;
        position << "light[" << i << "].position";
        std::ostringstream ambient;
        ambient << "light[" << i << "].ambient";
        std::ostringstream diffuse;
        diffuse << "light[" << i << "].diffuse";
        std::ostringstream specular;
        specular << "light[" << i << "].specular";

        shader.setVec3(position.str(), list_light[i].coord);
        shader.setVec3(ambient.str(), ambientColor);
        shader.setVec3(diffuse.str(), diffuseColor);
        shader.setVec3(specular.str(), list_light[i].lightSpecular);
        //shader.setFloat("light.constant", 0.5f);
        //shader.setFloat("light.linear", 0.009f);
        //shader.setFloat("light.quadratic", 0.0032f);
        //shader.setVec3("light.direction", camera.Front);
        //shader.setFloat("light.cutOff", glm::cos(glm::radians(30.0f)));
    }
}