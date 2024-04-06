#include "../Mesh.h"

Mesh createTube(int resolution, float height, float radius, vec3 coord, vec3 rotation) {

    float pitch = rotation.x;
    float yaw = rotation.y;
    float roll = rotation.z;

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(roll), glm::vec3(0, 0, 1));

    if (resolution < 2) {
        return {};
    }

    int latCount = 2; // Pour un cylindre, nous avons besoin seulement de deux "latitudes" pour les bords supérieur et inférieur
    int lonCount = resolution;

    const int totalVertexCount = (latCount + 1) * (lonCount + 1);
    const int totalIndexCount = 6 * lonCount * latCount;
    const float heightStep = height;
    const float lonStep = 2 * 3.1415927f / lonCount;

    Mesh cylinder;

    cylinder.index.resize(totalIndexCount);
    cylinder.pos.resize(totalVertexCount);
    cylinder.uvs.resize(totalVertexCount);
    cylinder.norm.resize(totalVertexCount);

    int currentVertex = 0;
    int currentIndex = 0;

    for (int lat = 0; lat <= latCount; lat++) {
        float currentHeight = lat * heightStep - height / 2.0f;

        for (int lon = 0; lon <= lonCount; lon++) {
            float x = cos(lon * lonStep) * radius;
            float y = sin(lon * lonStep) * radius;

            vec3 originalPos = vec3(x + coord.x, y + coord.y, currentHeight + coord.z);
            vec4 rotatedPos = rotationMatrix * vec4(originalPos, 1.0f);
            cylinder.pos[currentVertex] = vec3(rotatedPos);
            cylinder.norm[currentVertex] = normalize(vec3(x, y, 0.0));
            cylinder.uvs[currentVertex] = vec2((float)lon / lonCount, (float)lat / latCount);

            currentVertex++;
        }
    }

    for (int lat = 0; lat < latCount; lat++) {
        for (int lon = 0; lon < lonCount; lon++) {
            int first = (lat * (lonCount + 1)) + lon;
            int second = first + lonCount + 1;

            cylinder.index[currentIndex++] = first;
            cylinder.index[currentIndex++] = second;
            cylinder.index[currentIndex++] = first + 1;

            cylinder.index[currentIndex++] = second;
            cylinder.index[currentIndex++] = second + 1;
            cylinder.index[currentIndex++] = first + 1;
        }
    }

    cylinder.rotation = rotation;
    cylinder.coord = coord;

    return cylinder;
}
