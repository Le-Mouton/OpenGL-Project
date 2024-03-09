#ifndef LEARNINGPROJECT_MESHLOAD_H
#define LEARNINGPROJECT_MESHLOAD_H

#include "Mesh.h"
#include "Shader.h"

class MeshLoad{
public:
    unsigned int VAO;
    Mesh mesh;

    MeshLoad(Mesh &mesh){
        this->mesh = mesh;
        setUpMesh();
    }

    void Draw(Shader &shader)
    {
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.index.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }
    void Delete(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    unsigned int VBO, EBO;
    void setUpMesh(){

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        std::vector<float> bufferData;
        bufferData.reserve(mesh.pos.size() * 6);

        for(size_t i = 0; i < mesh.pos.size(); ++i) {
            bufferData.insert(bufferData.end(), {mesh.pos[i].x, mesh.pos[i].y, mesh.pos[i].z});
            bufferData.insert(bufferData.end(), {mesh.norm[i].x, mesh.norm[i].y, mesh.norm[i].z});
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);

        // Bind and set up EBO for indices as before
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index.size() * sizeof(int), mesh.index.data(), GL_STATIC_DRAW);

        GLsizei stride = 6 * sizeof(float);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif //LEARNINGPROJECT_MESHLOAD_H
