#ifndef LEARNINGPROJECT_MESHLOAD_H
#define LEARNINGPROJECT_MESHLOAD_H

#include "Mesh.h"
#include "Shader.h"
#include "Player.h"

class MeshLoad : public Mesh{
public:
    unsigned int VAO;
    Mesh mesh;

    MeshLoad(const Mesh& mesh){
        this->mesh = mesh;
        setUpMesh();
    }
    void UpdateMesh(vec3 coord){
        this->mesh.coord = coord;
    }
    void DrawObject(Shader &shader, Player &player, vec3 material_diffuse = vec3(1.0f), vec3 material_specular = vec3(1.0f), float shininess = 8.0f, float deltatime=0.0f)
    {
        shader.setMat4("view", player.getViewMatrix());
        shader.setMat4("projection", player.getProjection(1.77f));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->mesh.coord);
        model = glm::rotate(model, glm::radians(this->mesh.rotation.x), glm::vec3(1, 0, 0)); // Rotation autour de l'axe X
        model = glm::rotate(model, glm::radians(this->mesh.rotation.y), glm::vec3(0, 1, 0)); // Rotation autour de l'axe Y
        model = glm::rotate(model, glm::radians(this->mesh.rotation.z), glm::vec3(0, 0, 1)); // Rotation autour de l'axe Z
        shader.setMat4("model", model);
/*
        glm::vec3 lightColor = glm::vec3(1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.8f);

        shader.setVec3("light.position", glm::vec3(5.0f));
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", lightColor);
*/
        if(deltatime!=0.0f)
            shader.setFloat("deltatime", deltatime);
        shader.setVec3("viewPos", player.Position);
        shader.setVec3("material.diffuse", material_diffuse);
        shader.setVec3("material.specular", material_specular);
        shader.setFloat("material.shininess", shininess);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->mesh.index.size()), GL_UNSIGNED_INT, 0);
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
        bufferData.reserve(this->mesh.pos.size() * 6);

        for(size_t i = 0; i < this->mesh.pos.size(); ++i) {
            bufferData.insert(bufferData.end(), {this->mesh.pos[i].x, this->mesh.pos[i].y, this->mesh.pos[i].z});
            bufferData.insert(bufferData.end(), {this->mesh.norm[i].x, this->mesh.norm[i].y, this->mesh.norm[i].z});
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh.index.size() * sizeof(int), this->mesh.index.data(), GL_STATIC_DRAW);

        GLsizei stride = 6 * sizeof(float);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif //LEARNINGPROJECT_MESHLOAD_H
