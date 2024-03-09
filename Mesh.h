#ifndef LEARNINGPROJECT_MESH_H
#define LEARNINGPROJECT_MESH_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include <limits>

using namespace glm;

struct Mesh {
    std::vector<int> index;
    std::vector<vec3> pos;
    std::vector<vec2> uvs;
    std::vector<vec3> norm;
    vec3 rotation;
    vec3 coord;
};

Mesh MakePlane(int xCount, int yCount, vec3 coord, vec3 rotation, float width, float height);
Mesh MakeCapsule(int resolution, float height, float radius, vec3 coord, vec3 rotation);
Mesh MakeIcosphere(int resolution, float radius, vec3 coord);

#endif //LEARNINGPROJECT_MESH_H
