//
// Created by Guy on 20/08/2017.
//

#ifndef OMICRONRENDER_DECALRENDERER_HPP
#define OMICRONRENDER_DECALRENDERER_HPP

#include <render/ovr/TextureBuffer.hpp>
#include "RenderCommand.hpp"

namespace Omicron {

    class DecalRenderer {
    public:
        DecalRenderer(int width, int height);

        void RenderDecals(std::vector<RenderCommand> cmds, OVR::Matrix4f projection, OVR::Matrix4f view, ovr::TextureBuffer* textureBuffer, ovr::DepthBuffer* depthBuffer = nullptr);
        void RenderDecal(RenderCommand cmd, ovr::TextureBuffer* sampleBuffer, ovr::DepthBuffer* depthBuffer = nullptr);

    protected:
        void PopulateDecalBuffer(ovr::TextureBuffer* buffer);
        ovr::TextureBuffer* decalBuffer = nullptr;
        void RenderDecalCube();

        Shader decalShader;
        Shader pushBackShader;
        int width;
        int height;
        unsigned int decalCubeVAO = 0, decalCubeVBO = 0;
    };

}

#endif //OMICRONRENDER_DECALRENDERER_HPP
