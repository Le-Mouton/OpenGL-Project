#ifndef LEARNINGPROJECT_LIGHT_H
#define LEARNINGPROJECT_LIGHT_H

#include "Shader.h"

using namespace glm;

class Light{
public:
    Shader shader;
    float alpha;
    float beta;
    vec3 lightColor;
    vec3 lightSpecular;
    vec3 coord;
    Light(Shader shader, float alpha, float beta, vec3 lightColor, vec3 lightSpecular, vec3 coord, Shader shader1)
            : shader(shader1) {
        this->shader=shader;
        this->alpha=alpha;
        this->beta=beta;
        this->lightColor=lightColor;
        this->lightSpecular=lightSpecular;
        this->coord=coord;

        AddLight();
    }
    void AddLight(){
        glm::vec3 diffuseColor = lightColor   * glm::vec3(alpha);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(beta);

        shader.setVec3("light.position", coord);
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", lightSpecular);
        //shader.setFloat("light.constant", 0.5f);
        //shader.setFloat("light.linear", 0.009f);
        //shader.setFloat("light.quadratic", 0.0032f);
        //shader.setVec3("light.direction", camera.Front);
        //shader.setFloat("light.cutOff", glm::cos(glm::radians(30.0f)));
    }

};

#endif //LEARNINGPROJECT_LIGHT_H
