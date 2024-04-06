#include "../Mesh.h"

Mesh createTriangle(int resolution, int size, glm::vec3 coord, glm::vec3 rotation){
    Mesh flame;
    flame.coord = coord;
    flame.rotation = rotation;

    float xStep = size /resolution+1;
    float yStep = size /resolution+1;

    int xCount = resolution + 1;
    int yCount = resolution + 1;

    float offset = 0;
    for (int y = 0; y <= yCount; y++) {
        for (int y = 0; y <= yCount; y++) {
            for (int x = 0; x < xCount; x++) {
                glm::vec3 position(x * xStep + offset - (xStep * resolution) / 2, y * yStep, 0);
                flame.pos.push_back(position + coord);
                flame.norm.push_back(vec3(0.0f, 0.0f, 1.0f));
            }
            offset += xStep / 2;
            xCount -= 1;
        }
    }
        xCount = resolution + 1;

    int totalVertices = 0;
    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount - 1; x++) {
            int start = totalVertices + x;
            int nextRowStart = totalVertices + x + xCount;

            flame.index.push_back(start);
            flame.index.push_back(nextRowStart);
            flame.index.push_back(start + 1);

            if (x < xCount - 2) {
                flame.index.push_back(nextRowStart);
                flame.index.push_back(nextRowStart + 1);
                flame.index.push_back(start + 1);
            }
        }
        totalVertices += xCount;
        xCount--;
    }
    return flame;
}