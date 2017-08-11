//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_PRIMITIVERENDERER_HPP
#define OMICRONRENDER_PRIMITIVERENDERER_HPP

namespace Omicron {

    class PrimitiveRenderer {
    public:
        void RenderQuad();

    protected:
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;
    };

}

#endif //OMICRONRENDER_PRIMITIVERENDERER_HPP
