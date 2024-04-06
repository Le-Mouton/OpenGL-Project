#include "../Mesh.h"

Mesh createCylinder(int resolution, float height, float radius, vec3 coord, vec3 rotation) {

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

    int latCount = 2;
    int lonCount = resolution;
    int verticalResolution = 4*height;
    const int totalVertexCount = (lonCount + 1) * (verticalResolution +1) * 2 + 2; // +2 pour les centres des extrémités
    const int totalIndexCount = 6 * (lonCount) * verticalResolution * 2 + resolution * 6; // Faces latérales + disques d'extrémité
    const float lonStep = 2 * 3.1415927f / lonCount;
    const float heightStep = height / verticalResolution;

    Mesh cylinder;

    cylinder.index.resize(totalIndexCount);
    cylinder.pos.resize(totalVertexCount);
    cylinder.uvs.resize(totalVertexCount);
    cylinder.norm.resize(totalVertexCount);


    // Ajout des centres d'extrémité
    for (int i = 0; i < 3; i++) {
        float z = i * height - height / 2.0f + coord.z;

        vec3 centerPos = vec3(coord.x, coord.y, z);
        vec4 rotatedCenterPos = rotationMatrix * vec4(centerPos, 1.0f);
        cylinder.pos[i] = vec3(rotatedCenterPos); // Stocke les centres au début
        cylinder.norm[i] = vec3(0, 0, 1);
        cylinder.uvs[i] = vec2(0.5f, 0.5f);
    }

    int currentVertex = 2;
    int currentIndex = 0;

    for (int v = 0; v <=  verticalResolution; v++) {
        float currentHeight = v * heightStep - height / 2.0f + coord.z;
        for (int lon = 0; lon <= lonCount; lon++) {
            float x = cos(lon * lonStep) * radius;
            float y = sin(lon * lonStep) * radius;

            vec3 originalPos = vec3(x + coord.x, y + coord.y, currentHeight);
            vec4 rotatedPos = rotationMatrix * vec4(originalPos, 1.0f);

            cylinder.pos[currentVertex] = vec3(rotatedPos);
            if (currentHeight == -height/2 ||currentHeight == height/2)
                cylinder.norm[currentVertex] = normalize(vec3(0, 0, 1));
            else
                cylinder.norm[currentVertex] = normalize(vec3(x, y, 0.0));
            cylinder.uvs[currentVertex] = vec2((float)lon / lonCount, (float)v / verticalResolution);
            currentVertex++;
        }
    }

    for (int lat = 0; lat < latCount; lat++) {

        float currentHeight = lat * height - height / 2.0f + coord.z;

        for (int lon = 0; lon <= lonCount; lon++) {
            float x = cos(lon * lonStep) * radius;
            float y = sin(lon * lonStep) * radius;

            vec3 pos = vec3(x + coord.x, y + coord.y, currentHeight);
            vec4 rotatedPos = rotationMatrix * vec4(pos, 1.0f);

            cylinder.pos[currentVertex] = vec3(rotatedPos);
            if (currentHeight == -height/2 ||currentHeight == height/2)
                cylinder.norm[currentVertex] = normalize(vec3(0, 0, 1));
            else
                cylinder.norm[currentVertex] = normalize(vec3(x, y, 0.0));
            cylinder.uvs[currentVertex] = vec2((float)lon / lonCount, (float)lat / latCount);
            currentVertex++;
        }
    }

    // Indexation pour les disques d'extrémité
    for (int i = 0; i < 2; i++) {
        int centerIndex = i; // 0 pour le bas, 1 pour le haut
        int startIndex = 2 + i * (lonCount + 1) * (verticalResolution); // Début du bord pour chaque extrémité
        for (int lon = 0; lon <= lonCount; lon++) {

            int first = startIndex + lon;
            int second = startIndex + ((lon + 1) % lonCount);

            if (i == 0) { // Bas, orienté vers l'extérieur
                cylinder.index[currentIndex++] = centerIndex;
                cylinder.index[currentIndex++] = first;
                cylinder.index[currentIndex++] = second;
            } else { // Haut, orienté vers l'extérieur
                cylinder.index[currentIndex++] = centerIndex;
                cylinder.index[currentIndex++] = second;
                cylinder.index[currentIndex++] = first;
            }
        }
    }
    // Indexation des côtés du cylindre
    for (int v = 0; v < verticalResolution; v++) {
        for (int lon = 0; lon <= lonCount; lon++) {
            int current = v * (lonCount +1) + lon +2;
            int next = current + lonCount -1;

            cylinder.index[currentIndex++] = next;
            cylinder.index[currentIndex++] = next + 1;
            cylinder.index[currentIndex++] = current + 1;

            cylinder.index[currentIndex++] = current;
            cylinder.index[currentIndex++] = next;
            cylinder.index[currentIndex++] = current + 1;
        }
    }


    cylinder.rotation = rotation;
    cylinder.coord = coord;

    return cylinder;
}
