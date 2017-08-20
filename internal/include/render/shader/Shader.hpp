//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_SHADER_HPP
#define OMICRONRENDER_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

namespace Omicron {

    class Shader {
    public:
        GLuint ID;
        Shader() {}
        Shader &Use();

        int Find(std::string name);

        virtual ~Shader();

        void SetSources(std::string vertSource, std::string fragSource, std::string geomSource = "");
        void SetSourcesFromFiles(std::string vertFile, std::string fragFile, std::string geomFile = "");
        void CompileFromSources();

        void CompileFromFile(std::string vertFile, std::string fragFile, std::string geomFile = "");
        void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

        void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
        void SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
        void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
        void SetVector2f(const GLchar *name, glm::vec2 &value, GLboolean useShader = false);
        void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
        void SetVector3f(const GLchar *name, glm::vec3 &value, GLboolean useShader = false);
        void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
        void SetVector4f(const GLchar *name, glm::vec4 &value, GLboolean useShader = false);
        void SetMatrix4(const GLchar *name, glm::mat4 &matrix, GLboolean useShader = false);


    private:
        void CheckCompileErrors(GLuint object, std::string type);
        std::string vertSource;
        std::string fragSource;
        std::string geomSource;

        bool dirty;
    };

    namespace {
        Shader* activeShader = nullptr;
    }

};

#endif //OMICRONRENDER_SHADER_HPP
