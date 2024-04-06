#include "../Mesh.h"
#include <unordered_map>

static const float Z = (1.0f + sqrt(5.0f)) / 2.0f;
static const vec2 UV = vec2(1 / 11.0f, 1 / 3.0f);

static const int IcoVertexCount = 22;
static const int IcoIndexCount = 60;

static const vec3 IcoVerts[] = {
        vec3( 0, -1, -Z), vec3(-1, -Z,  0), vec3( Z,  0, -1), vec3( 1, -Z,  0),
        vec3( 1,  Z,  0), vec3(-1, -Z,  0), vec3( Z,  0,  1), vec3( 0, -1,  Z),
        vec3( 1,  Z,  0), vec3(-1, -Z,  0), vec3( 0,  1,  Z), vec3(-Z,  0,  1),
        vec3( 1,  Z,  0), vec3(-1, -Z,  0), vec3(-1,  Z,  0), vec3(-Z,  0, -1),
        vec3( 1,  Z,  0), vec3(-1, -Z,  0), vec3( 0,  1, -Z), vec3( 0, -1, -Z),
        vec3( 1,  Z,  0), vec3( Z,  0, -1)
};

static const vec2 IcoUvs[] = {
        UV * vec2( 0, 1),
        UV * vec2( 1, 0),
        UV * vec2( 1, 2),
        UV * vec2( 2, 1),
        UV * vec2( 2, 3),
        UV * vec2( 3, 0),
        UV * vec2( 3, 2),
        UV * vec2( 4, 3),
        UV * vec2( 5, 0),
        UV * vec2( 5, 2),
        UV * vec2( 6, 1),
        UV * vec2( 6, 3),
        UV * vec2( 7, 0),
        UV * vec2( 7, 2),
        UV * vec2( 8, 1),
        UV * vec2( 8, 3),
        UV * vec2( 9, 0),
        UV * vec2( 9, 2),
        UV * vec2(10, 1),
        UV * vec2(10, 3),
        UV * vec2(11, 2)
};

static const int IcoIndex[] = {
        2,  6,  4,
        6, 10,  8,
        10, 14, 12,
        14, 18, 16,
        18, 21, 20,

        0,  3,  2,
        2,  3,  6,
        3,  7,  6,
        6,  7, 10,
        7, 11, 10,
        10, 11, 14,
        11, 15, 14,
        14, 15, 18,
        15, 19, 18,
        18, 19, 21,

        0,  1,  3,
        3,  5,  7,
        7,  9, 11,
        11, 13, 15,
        15, 17, 19
};

Mesh createIcosphere(int resolution, float radius, vec3 coord) {

    const int rn = (int)pow(4, resolution);
    const int totalIndexCount = IcoIndexCount * rn;
    const int totalVertexCount = IcoVertexCount + IcoIndexCount * (1 - rn) / (1 - 4);

    Mesh sphere;

    sphere.index.resize(totalIndexCount);
    sphere.pos.resize(totalVertexCount);
    sphere.uvs.resize(totalVertexCount);
    sphere.coord = coord;

    for (int i = 0; i < IcoVertexCount; i++) {
        sphere.pos[i] = IcoVerts[i];
        sphere.uvs[i] = IcoUvs[i];
    }

    for (int i = 0; i < IcoIndexCount; i++) {
        sphere.index[i] = IcoIndex[i];
    }

    int currentIndexCount = IcoIndexCount;
    int currentVertCount = IcoVertexCount;

    for (int r = 0; r < resolution; r++) {

        std::unordered_map<uint64_t, int> triangleFromEdge;
        int indexCount = currentIndexCount;

        for (int t = 0; t < indexCount; t += 3) {
            int midpoints[3] = {};

            for (int e = 0; e < 3; e++) {
                int first = sphere.index[t + e];
                int second = sphere.index[t + (t + e + 1) % 3];

                if (first > second) {
                    std::swap(first, second);
                }

                uint64_t hash = (uint64_t)first | (uint64_t)second << (sizeof(uint32_t) * 8);

                auto result = triangleFromEdge.insert({ hash, currentVertCount });
                auto& triangle = result.first;
                auto wasNewEdge = result.second;

                if (wasNewEdge) {
                    sphere.pos[currentVertCount] = (sphere.pos[first] + sphere.pos[second]) / 2.0f;
                    sphere.uvs[currentVertCount] = (sphere.uvs[first] + sphere.uvs[second]) / 2.0f;

                    currentVertCount += 1;
                }

                midpoints[e] = triangle->second;
            }

            int mid0 = midpoints[0];
            int mid1 = midpoints[1];
            int mid2 = midpoints[2];

            sphere.index[currentIndexCount++] = sphere.index[t];
            sphere.index[currentIndexCount++] = mid0;
            sphere.index[currentIndexCount++] = mid2;

            sphere.index[currentIndexCount++] = sphere.index[t + 1];
            sphere.index[currentIndexCount++] = mid1;
            sphere.index[currentIndexCount++] = mid0;

            sphere.index[currentIndexCount++] = sphere.index[t + 2];
            sphere.index[currentIndexCount++] = mid2;
            sphere.index[currentIndexCount++] = mid1;

            sphere.index[t]     = mid0;
            sphere.index[t + 1] = mid1;
            sphere.index[t + 2] = mid2;
        }
    }


    for (size_t i = 0; i < sphere.pos.size(); i++) {
        vec3& pos = sphere.pos[i];
        pos = normalize(pos); // Normaliser pour obtenir une direction uniforme
        sphere.norm.push_back(pos); // Utiliser la direction normalisée comme normale
        pos = pos * radius; // Ajuster selon le rayon et ajouter les coordonnées
    }

    sphere.rotation = vec3(0.0f);
    return sphere;
}