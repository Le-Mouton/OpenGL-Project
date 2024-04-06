#include "../Mesh.h"

Mesh createPlane(int xCount, int yCount, vec3 coord, vec3 rotation, float width, float height) {

    float pitch = rotation.x;
    float yaw = rotation.y;
    float roll = rotation.z;

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(roll), glm::vec3(0, 0, 1));


    if (xCount == 0 || yCount == 0) {
        return {};
    }

    const int totalIndexCount = 6 * xCount * yCount;
    const int totalVertexCount = (xCount + 1) * (yCount + 1);

    const vec3 offset = vec3(-width / 2.0f, 0, -height / 2.0f);

    const float xStep = width / xCount;
    const float yStep = height / yCount;

    const float uStep = 1.0f / xCount;
    const float vStep = 1.0f / yCount;

    Mesh plane;

    plane.index.resize(totalIndexCount);
    plane.pos.resize(totalVertexCount);
    plane.uvs.resize(totalVertexCount);

    vec3 originalNorm = vec3(0, 1, 0);
    vec4 rotatedNorm = rotationMatrix * vec4(originalNorm, 1.0f);

    plane.norm.resize(totalVertexCount, vec3(rotatedNorm));

    for (int x = 0; x <= xCount; x++)
        for (int y = 0; y <= yCount; y++)
        {
            int i = y + x * (yCount + 1);

            vec3 originalPos = vec3(x * xStep, 0, y * yStep) + offset + coord;
            vec4 rotatedPos = rotationMatrix * vec4(originalPos, 1.0f); // Applique la rotation

            plane.pos[i] = vec3(rotatedPos);
            plane.uvs[i] = vec2(x * uStep, (yCount - y) * vStep);
        }

    int i = 0;
    for (int v = 0; v < totalVertexCount - yCount - 2; v++) {
        if ((v + 1) % (yCount + 1) == 0) {
            v++;
        }

        plane.index[i++] = v;
        plane.index[i++] = v + 1;
        plane.index[i++] = v + yCount + 1;

        plane.index[i++] = v + 1;
        plane.index[i++] = v + yCount + 2;
        plane.index[i++] = v + yCount + 1;
    }

    plane.rotation = vec3(pitch, yaw, roll);
    plane.coord = coord;
    return plane;
}
