//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_PRIMITIVERENDERER_HPP
#define OMICRONRENDER_PRIMITIVERENDERER_HPP

namespace Omicron {

    class PrimitiveRenderer {
    public:
        void RenderQuad();
        unsigned int BlankTexture();

        void RenderSkybox();

    protected:
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;

        unsigned int skyboxVAO = 0;
        unsigned int skyboxVBO = 0;

        unsigned int texId = 0;
    };

}

#endif //OMICRONRENDER_PRIMITIVERENDERER_HPP
