//
// Created by Guy on 07/08/2017.
//

#include <data/UBO.hpp>
#include <pch.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace Omicron {

    UBO::UBO(unsigned int usage, std::string key) : usage(usage), key(std::move(key)) {}

    void UBO::LinkToShader(Shader& shader) {
        unsigned int blockIndex = glGetUniformBlockIndex(shader.ID, key.c_str());
        glUniformBlockBinding(shader.ID, blockIndex, 0);
        blockIndices.push_back(blockIndex);
    }

    void UBO::Allocate(size_t size) {
        glGenBuffers(1, &block);
        Reallocate(size);
    }

    void UBO::Reallocate(size_t size) {
        glBindBuffer(GL_UNIFORM_BUFFER, block);
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, usage);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, block);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UBO::Discard() {
        glDeleteBuffers(1, &block);
    }

    void UBO::SetUniformMatrix(glm::mat4 matrix, size_t size, size_t offset) {
        glBindBuffer(GL_UNIFORM_BUFFER, block);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, glm::value_ptr(matrix));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
};