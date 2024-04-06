#include "../Mesh.h"

Mesh createRectangle(int xCount, int yCount, int zCount, vec3 coord, vec3 rotation, float width, float height, float depth){
    Mesh rectangle;
    if (xCount <= 0 || yCount <= 0 || zCount <= 0) return rectangle;

    float pitch = rotation.x;
    float yaw = rotation.y;
    float roll = rotation.z;

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(roll), glm::vec3(0, 0, 1));

    // Step size for each dimension
    float xStep = width / xCount;
    float yStep = height / yCount;
    float zStep = depth / zCount;

    // Generate vertices
    for (int y = 0; y <= yCount; ++y) {
        for (int x = 0; x <= xCount; ++x) {
            glm::vec3 pos(x * xStep - width/2 + coord.x, y * yStep - height/2 + coord.y, -depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(0, 0, -1);
            glm::vec2 uv(x / (float)xCount, y / (float)yCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    for (int y = 0; y <= yCount; ++y) {
        for (int x = 0; x <= xCount; ++x) {
            glm::vec3 pos(x * xStep - width/2 + coord.x, y * yStep - height/2 + coord.y, depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(0, 0, 1);
            glm::vec2 uv(x / (float)xCount, y / (float)yCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    for (int z = 0; z <= zCount; ++z) {
        for (int x = 0; x <= xCount; ++x) {
            glm::vec3 pos(x * xStep - width/2 + coord.x, -height/2 + coord.y, z * zStep - depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(0, -1, 0);
            glm::vec2 uv(x / (float)xCount, z / (float)zCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    for (int z = 0; z <= zCount; ++z) {
        for (int x = 0; x <= xCount; ++x) {
            glm::vec3 pos(x * xStep - width/2 + coord.x, height/2 + coord.y, z * zStep - depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(0, 1, 0);
            glm::vec2 uv(x / (float)xCount, z / (float)zCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    for (int z = 0; z <= zCount; ++z) {
        for (int y = 0; y <= yCount; ++y) {

            glm::vec3 pos(-width/2 + coord.x, y * yStep - height/2 + coord.y, z * zStep - depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(-1, 0, 0);
            glm::vec2 uv(y / (float)xCount, z / (float)zCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    for (int z = 0; z <= zCount; ++z) {
        for (int y = 0; y <= yCount; ++y) {

            glm::vec3 pos(width/2 + coord.x, y * yStep - height/2 + coord.y, z * zStep-depth/2 + coord.z);
            glm::vec3 worldPos = glm::vec3(rotationMatrix * glm::vec4(pos, 1.0));
            glm::vec3 norm = glm::vec3(1, 0, 0);
            glm::vec2 uv(y / (float)xCount, z / (float)zCount);

            rectangle.pos.push_back(worldPos);
            rectangle.norm.push_back(norm);
            rectangle.uvs.push_back(uv);

        }
    }
    // Generate indices
    for (int z = 0; z < zCount; ++z) {
        for (int y = 0; y < yCount; ++y) {
            for (int x = 0; x < xCount; ++x) {
                int start = x + y * (xCount + 1) + z * (xCount + 1) * (yCount + 1);
                rectangle.index.push_back(start);
                rectangle.index.push_back(start + 1);
                rectangle.index.push_back(start + xCount + 2);

                rectangle.index.push_back(start);
                rectangle.index.push_back(start + xCount + 2);
                rectangle.index.push_back(start + xCount + 1);
            }
        }
    }

    rectangle.coord = coord;
    rectangle.rotation = rotation;

    return rectangle;
}
