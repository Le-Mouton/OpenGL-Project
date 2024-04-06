#include "../Mesh.h"
#include <float.h>

Mesh createCapsule(int resolution, float height, float radius, vec3 coord, vec3 rotation) {

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

    int latCount = resolution;
    int lonCount = resolution;

    if (latCount % 2 == 0) {
        latCount++;
    }

    const int totalIndexCount = 6 * (latCount - 1) * lonCount;
    const int totalVertexCount = (latCount + 1) * (lonCount + 1);
    const float latStep = 3.1415927f / latCount;
    const float lonStep = 2 * 3.1415927f / lonCount;
    const float zOffset = clamp(height / 2.0f - radius, 0.0f, FLT_MAX);

    Mesh capsule;

    capsule.index.resize(totalIndexCount);
    capsule.pos.resize(totalVertexCount);
    capsule.uvs.resize(totalVertexCount);
    capsule.norm.resize(totalVertexCount);

    int currentVertex = 0;
    int currentIndex = 0;

    for (int lat = 0; lat <= latCount; lat++) {
        float offset = lat > latCount / 2 ? zOffset : -zOffset;

        for (int lon = 0; lon <= lonCount; lon++) {

            vec3 originalPos = vec3(
                    cos(lon * lonStep) * sin(lat * latStep) * radius + coord.x,
                    sin(lon * lonStep) * sin(lat * latStep) * radius + coord.y,
                    cos(lat * latStep - 3.1415927f) * radius + offset + coord.z
            );
            vec4 rotatedPos = rotationMatrix * vec4(originalPos, 1.0f);
            capsule.pos[currentVertex] = vec3(rotatedPos);

            //norm
            float x = cos(lon * lonStep) * sin(lat * latStep);
            float y = sin(lon * lonStep) * sin(lat * latStep);
            float z = cos(lat * latStep);

            // Pour le cylindre
            if(lat > latCount / 4 && lat < 3 * latCount / 4) {
                capsule.norm[currentVertex] = normalize(vec3(x, y, 0.0));
            } else { // Pour les hémisphères
                capsule.norm[currentVertex] = normalize(vec3(x, y, z));
            }

            capsule.uvs[currentVertex] = vec2(
                    (float)lon / lonCount,
                    capsule.pos[currentVertex].z / (radius * 2 + height) + 0.5f
            );

            currentVertex ++;
        }
    }

    int v = lonCount + 1;
    for (int lon = 0; lon < lonCount; lon++) {
        capsule.index[currentIndex++] = lon;
        capsule.index[currentIndex++] = v;
        capsule.index[currentIndex++] = v + 1;

        v += 1;
    }

    v = lonCount + 1;
    for (int lat = 1; lat < latCount - 1; lat++) {
        for (int lon = 0; lon < lonCount; lon++) {
            capsule.index[currentIndex++] = v;
            capsule.index[currentIndex++] = v + lonCount + 1;
            capsule.index[currentIndex++] = v + 1;

            capsule.index[currentIndex++] = v + 1;
            capsule.index[currentIndex++] = v + lonCount + 1;
            capsule.index[currentIndex++] = v + lonCount + 2;

            v += 1;
        }

        v += 1;
    }

    for (int lon = 0; lon < lonCount; lon++) {
        capsule.index[currentIndex++] = v;
        capsule.index[currentIndex++] = v + lonCount + 1;
        capsule.index[currentIndex++] = v + 1;

        v += 1;
    }

    capsule.rotation = rotation;
    capsule.coord = coord;

    return capsule;
}