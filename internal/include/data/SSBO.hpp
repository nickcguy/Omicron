//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_SSBO_HPP
#define OMICRONRENDER_SSBO_HPP

#include <string>
#include <render/shader/Shader.hpp>
#include <vector>

namespace Omicron {

    class SSBO {
    public:
        SSBO(unsigned int usage, std::string key);
        void LinkToShader(Shader& shader);

        void Allocate();
        void SetData(size_t size, void* data);
        void Map();

    protected:
        size_t size;
        void* data;

        unsigned int block;
        unsigned int blockIndex;
        unsigned int bindingPointIndex = 0;
        unsigned int usage;
        unsigned int id;
        std::string key;
        std::vector<unsigned int> blockIndices;

    };

}

#endif //OMICRONRENDER_SSBO_HPP
