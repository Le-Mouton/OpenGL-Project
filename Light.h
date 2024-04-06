#ifndef LEARNINGPROJECT_LIGHT_H
#define LEARNINGPROJECT_LIGHT_H

#include "Shader.h"
#include <iostream>
#include <vector>

using namespace glm;

class Light{
public:
    float alpha;
    float beta;
    vec3 lightColor;
    vec3 lightSpecular;
    vec3 coord;


    Light(float alpha, float beta, vec3 lightColor, vec3 lightSpecular, vec3 coord);

};
void AddLight(std::vector <Light> list_light, Shader &shader);

#endif //LEARNINGPROJECT_LIGHT_H
