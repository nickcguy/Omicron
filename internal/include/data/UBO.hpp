//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_UBO_HPP
#define OMICRONRENDER_UBO_HPP

#include <cstdlib>
#include <vector>
#include <render/shader/Shader.hpp>

namespace Omicron {

    class UBO {
    public:
        UBO(unsigned int usage, std::string key);

        void LinkToShader(Shader& shader);
        void Allocate(size_t size);
        void Reallocate(size_t size);
        void Discard();

        void SetUniformMatrix(glm::mat4 data, size_t size, size_t offset);

    protected:
        unsigned int block;
        unsigned int usage;
        std::string key;
        std::vector<unsigned int> blockIndices;

    };

}

#endif //OMICRONRENDER_UBO_HPP
