//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_CUBEMAP_HPP
#define OMICRONRENDER_CUBEMAP_HPP

#include <pch.hpp>
#include <string>
#include <vector>

namespace Omicron {

    class Cubemap {
    public:
        static int LoadCubemap(std::vector<std::string> faces);
    };

}

#endif //OMICRONRENDER_CUBEMAP_HPP
