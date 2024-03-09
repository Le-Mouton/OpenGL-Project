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
    void DrawObject(Shader &shader, Camera &camera, vec3 material_diffuse = vec3(1.0f), vec3 material_specular = vec3(1.0f), float shininess = 16.0f)
    {
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjection(1.77f));
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("material.diffuse", material_diffuse);
        shader.setVec3("material.specular", material_specular);
        shader.setFloat("material.shininess", shininess);
        glm::mat4 modelSphere = glm::mat4(1.0f);
        modelSphere = glm::translate(modelSphere, mesh.coord);
        modelSphere = glm::rotate(modelSphere, glm::radians(mesh.rotation.x), glm::vec3(1, 0, 0)); // Rotation autour de l'axe X
        modelSphere = glm::rotate(modelSphere, glm::radians(mesh.rotation.y), glm::vec3(0, 1, 0)); // Rotation autour de l'axe Y
        modelSphere = glm::rotate(modelSphere, glm::radians(mesh.rotation.z), glm::vec3(0, 0, 1)); // Rotation autour de l'axe Z
        shader.setMat4("model", modelSphere);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.index.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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
        glBindVertexArray(VAO);

        std::vector<float> bufferData;
        bufferData.reserve(mesh.pos.size() * 6);

        for(size_t i = 0; i < mesh.pos.size(); ++i) {
            bufferData.insert(bufferData.end(), {mesh.pos[i].x, mesh.pos[i].y, mesh.pos[i].z});
            bufferData.insert(bufferData.end(), {mesh.norm[i].x, mesh.norm[i].y, mesh.norm[i].z});
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index.size() * sizeof(int), mesh.index.data(), GL_STATIC_DRAW);

        GLsizei stride = 6 * sizeof(float);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif //LEARNINGPROJECT_MESHLOAD_H
