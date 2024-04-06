#ifndef LEARNINGPROJECT_SHADER_H
#define LEARNINGPROJECT_SHADER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/geometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
public:
    // the program id
    unsigned int ID;

    // the constructor of the class
    //    Shader(const char* vertexPath, const char* fragmentPath);

    //    void useShader();
    //	  void deleteShader();
    //usefull function
    //    void setBool(const std::string& name, bool value) const;
    //    void setFloat(const std::string& name, float value) const;
    //    void setInt(const std::string& name, int value) const;


    Shader(const char* vertexPath, const char* fragmentPath) {

        // 1. On récupère le contenu des deux fichier shader
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        //on verifie que ifstream fonctionne bien
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            //ouvre les fichiers
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            std::stringstream vertexShaderStream, fragmentShaderStream;
            //on lit les fichiers pour récupérer en string
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf(); // => rdbuf() récupère le contenu du fichier PAS ENCORE sous forme de string
            // on ferme les fichiers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // on les convertis en string
            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "Erreur lors de l'ouverture des shaders"<< e.what() << std::endl;
        }
        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();

        //------------------------
        // here we will create our shader program
        //------------------------

        //here we will create from our const just above
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        //then we open our shader and compile it
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        // optional but we can check if there are some errors when the shader is compiled
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Erreur de compilation du shader vertex!\n" << infoLog << std::endl;
        };
        //Now we do the same for fragment shader it's about the color
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Erreur de compilation du shader fragment\n" << infoLog << std::endl;
        };

        // We need a program to link these two shader
        ID = glCreateProgram();

        // we attach the saders by their ID to the shaderProgram
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        //as we did before we can cehck for an error
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Erreur lors du lien entre les shaders\n" << infoLog << std::endl;
        };

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    void useShader() {
        glUseProgram(ID);
    }
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string& name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }
    void setMat4(const std::string& name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    void deleteShader() {
        glDeleteProgram(ID);
    }
};

#endif //LEARNINGPROJECT_SHADER_H
