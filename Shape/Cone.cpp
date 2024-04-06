#include "../Mesh.h"

Mesh createCone(int resolution, float base, float height, glm::vec3 coord, glm::vec3 rotation) {
    Mesh cone;
    cone.coord = coord;
    cone.rotation = rotation;

    int apexIndex = 0;
    cone.pos.push_back(glm::vec3(0, height, 0) + coord);
    cone.uvs.push_back(glm::vec2(0.5f, 1.0f));
    cone.norm.push_back(glm::vec3(0, 1, 0));

    for(int i = 0; i < resolution; ++i) {
        float angle = 2.0f * 3.14 * i / resolution;
        float x = base * cos(angle);
        float z = base * sin(angle);
        cone.pos.push_back(glm::vec3(x, 0, z) + coord);
        cone.uvs.push_back(glm::vec2(x / (base / 2.0f) + 0.5f, z / (base / 2.0f) + 0.5f));
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
        cone.norm.push_back(normal);
    }

    for(int i = 0; i < resolution; ++i) {
        cone.index.push_back(apexIndex);
        cone.index.push_back(1 + i % resolution);
        cone.index.push_back(1 + (i + 1) % resolution);
    }

    int baseCenterIndex = cone.pos.size();
    cone.pos.push_back(glm::vec3(0, 0, 0) + coord); // Centre de la base
    cone.uvs.push_back(glm::vec2(0.5f, 0.5f));
    cone.norm.push_back(glm::vec3(0, -1, 0)); // Normale vers le bas

    for(int i = 0; i < resolution; ++i) {
        cone.index.push_back(baseCenterIndex);
        cone.index.push_back(1 + (i + 1) % resolution);
        cone.index.push_back(1 + i % resolution);
    }

    return cone;
}
