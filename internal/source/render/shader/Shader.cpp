//
// Created by Guy on 06/08/2017.
//

#include <render/shader/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <utils/FileUtils.hpp>
#include <utils/TextUtils.hpp>

#define SHADER_VERBOSE_ERROR true

namespace Omicron {

    Shader &Shader::Use() {
        if(dirty)
            CompileFromSources();
//        if(activeShader != this) {
//            activeShader = this;
            glUseProgram(this->ID);
//        }
        return *this;
    }

    void Shader::CompileFromFile(std::string vertFile, std::string fragFile, std::string geomFile) {
        std::string vert = Files::ReadFile(std::move(vertFile));
        std::string frag = Files::ReadFile(std::move(fragFile));
        std::string geom = geomFile.empty() ? "" : Files::ReadFile(geomFile);

        Compile(vert.c_str(), frag.c_str(), geom.empty() ? nullptr : geom.c_str());
    }

    void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource) {
        Utils::CheckErrors("Out of scope");

        this->vertSource = vertexSource;
        this->fragSource = fragmentSource;
        if(geometrySource != nullptr)
            this->geomSource = geometrySource;

        GLuint sVertex, sFragment, gShader;
        // Vertex Shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        Utils::CheckErrors("glCompileShader::VERTEX");
        this->CheckCompileErrors(sVertex, "VERTEX");
        // Fragment Shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        Utils::CheckErrors("glCompileShader::FRAGMENT");
        this->CheckCompileErrors(sFragment, "FRAGMENT");
        // If geometry shader source code is given, also compile geometry shader
        if (geometrySource != nullptr)  {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, NULL);
            glCompileShader(gShader);
            this->CheckCompileErrors(gShader, "GEOMETRY");
        }
        // Shader Program
        this->ID = glCreateProgram();
        Utils::CheckErrors("glCreateProgram");

        glAttachShader(this->ID, sVertex);
        glAttachShader(this->ID, sFragment);
        if (geometrySource != nullptr)
            glAttachShader(this->ID, gShader);
        glLinkProgram(this->ID);
        this->CheckCompileErrors(this->ID, "PROGRAM");
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr)
            glDeleteShader(gShader);

        dirty = false;
    }

    void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform1f(glGetUniformLocation(this->ID, name), value);
    }

    void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform1i(glGetUniformLocation(this->ID, name), value);

    }

    void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), x, y);
    }

    void Shader::SetVector2f(const GLchar *name, glm::vec2 &value, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
    }

    void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
    }

    void Shader::SetVector3f(const GLchar *name, glm::vec3 &value, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
    }

    void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
    }

    void Shader::SetVector4f(const GLchar *name, glm::vec4 &value, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMatrix4(const GLchar *name, glm::mat4 &matrix, GLboolean useShader) {
        if(useShader)
            this->Use();
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

// PRIVATE

    void Shader::CheckCompileErrors(GLuint object, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM") {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }else{
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }

        #if SHADER_VERBOSE_ERROR
        if(!success) {
            printf("VERTEX: \n%s\n", vertSource.c_str());
            printf("FRAGMENT: \n%s\n", fragSource.c_str());
        }
        #endif
    }

    Shader::~Shader() {
        glDeleteShader(this->ID);
    }

    void Shader::SetSources(std::string vertSource, std::string fragSource, std::string geomSource) {
        this->vertSource = vertSource;
        this->fragSource = fragSource;
        this->geomSource = geomSource;

        dirty = true;
    }

    void Shader::SetSourcesFromFiles(std::string vertFile, std::string fragFile, std::string geomFile) {
        std::string vert = Files::ReadFile(std::move(vertFile));
        std::string frag = Files::ReadFile(std::move(fragFile));
        std::string geom = geomFile.empty() ? "" : Files::ReadFile(geomFile);

        SetSources(vert, frag, geom);
    }

    void Shader::CompileFromSources() {
        Compile(vertSource.c_str(), fragSource.c_str(), geomSource.empty() ? nullptr : geomSource.c_str());
    }


};