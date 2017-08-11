//
// Created by Guy on 07/08/2017.
//

#include <data/SSBO.hpp>

namespace Omicron {

    SSBO::SSBO(unsigned int usage, std::string key) : usage(usage), key(key) {}

    void SSBO::Allocate() {
        glGenBuffers(1, &block);
    }

    void SSBO::LinkToShader(Shader& shader) {
        unsigned int blockIndex = glGetProgramResourceIndex(shader.ID, GL_SHADER_STORAGE_BLOCK, key.c_str());
        glShaderStorageBlockBinding(shader.ID, blockIndex, bindingPointIndex);


//        unsigned int blockIndex = glGetUniformBlockIndex(shader.ID, key.c_str());
//        glUniformBlockBinding(shader.ID, blockIndex, 0);
//        blockIndices.push_back(blockIndex);
    }

    void SSBO::SetData(size_t size, void* data) {
        this->size = size;
        this->data = data;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, block);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);

        GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        memcpy(p, data, size);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void SSBO::Map() {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, block);
    }

}