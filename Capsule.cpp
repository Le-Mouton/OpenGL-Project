#include "Mesh.h"
#include <float.h>

Mesh MakeCapsule(int resolution, float height, float radius, vec3 coord, vec3 rotation) {

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

    // Almost same generation as UV sphere but we force the
    // lat count to be odd so it can be split evenly

    int latCount = resolution;
    int lonCount = resolution;

    if (latCount % 2 == 0) {
        latCount++;
    }

    // Each longitudinal count makes two triangles (6 indices) for every
    // lateral count except for the top and bottom poles, which only make
    // one triangle per longitudinal count.

    // UV maps require two vertices with the same position, but different UVs
    // so we need counts + 1.

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


            // UVs are almost the same as the UV sphere, but V needs
            // to be scaled to fit the height

            capsule.uvs[currentVertex] = vec2(
                    (float)lon / lonCount,
                    capsule.pos[currentVertex].z / (radius * 2 + height) + 0.5f
            );

            currentVertex ++;
        }
    }

    // Top cap
    //
    // One triangle connects the first lateral layer to the second per longitudinal count.
    // Even though the top points all have the same position, their UVs are different,
    // so each triangle uses a different point.
    //
    //          -------- lonCount -------->
    //                      lon
    //    |                  *
    //    |                / | \
	//    1             /    |    \
	//    |          /       |       \
	//    |       /                     \
	//   \ /     *------------*------------*
    //          v          v + 1      (v + 1) + 1

    int v = lonCount + 1;
    for (int lon = 0; lon < lonCount; lon++) {
        capsule.index[currentIndex++] = lon;
        capsule.index[currentIndex++] = v;
        capsule.index[currentIndex++] = v + 1;

        v += 1;
    }

    // Middle
    //
    // Each lateral layer has 2 triangles for every longitudinal count.
    //
    //          -------- lonCount -------->
    //          v          v + 1      (v + 1) + 1
    //    |     *------------*------------*
    //    |     |          / |          / |
    // latCount |       /    |       /    |
    //    |     |    /       |    /       |
    //    |     | /          | /          |
    //   \ /    *------------*------------*
    //      v + lc + 1   v + lc + 2   (v + 1) + lc + 2

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

    // Bottom cap

    // Same as top cap, but reversed.
    //
    //          -------- lonCount -------->
    //          v          v + 1      (v + 1) + 1
    //    |     *------------*------------*
    //    |       \          |          /
    //    1          \       |       /
    //    |             \    |    /
    //    |                \ | /
    //   \ /                 *
    //                   v + lc + 1

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