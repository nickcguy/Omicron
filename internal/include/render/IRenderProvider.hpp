//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_IRENDERPROVIDER_HPP
#define OMICRONRENDER_IRENDERPROVIDER_HPP

#include <vector>
#include "RenderCommand.hpp"

namespace Omicron {

    class IRenderProvider {
    public:
        virtual std::vector<RenderCommand> Renderables();
        virtual void Renderables(std::vector<RenderCommand>&)=0;
    };

}

#endif //OMICRONRENDER_IRENDERPROVIDER_HPP