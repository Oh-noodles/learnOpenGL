#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath) {
            std::cout << "vertexPath: " << vertexPath << std::endl;
            std::cout << "fragmentPath: " << fragmentPath << std::endl;
            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            // fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment); 
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            std::cout << "========= shader initialized =========" << std::endl;
        }

        void use() {
            glUseProgram(ID);
        }

        void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setVec3(const std::string &name, glm::vec3 value) {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
        }

        void setVec3(const std::string &name, float x, float y, float z) {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
        }

        void setMat4(const std::string &name, glm::mat4 value) {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
    private:
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
};
#endif
