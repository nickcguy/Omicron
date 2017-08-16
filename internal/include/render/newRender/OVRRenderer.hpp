//
// Created by Guy on 14/08/2017.
//

#ifndef OMICRONRENDER_NEW_OVRRENDERER_HPP
#define OMICRONRENDER_NEW_OVRRENDERER_HPP

#include "BaseRenderer.hpp"
#include <OVR_CAPI.h>
#include <Extras/OVR_Math.h>
#include "../ovr/TextureBuffer.hpp"

namespace Omicron {

    class OVRContext;

    class OVRRenderer : public BaseRenderer {
    public:
        OVRRenderer(OpenGLContext* context);

        virtual void Init() override;
        void InitOVR();

        virtual void Update(float delta) override;

        virtual void Render(std::vector<RenderCommand> cmds) override;
        virtual void Render(RenderCommand cmd, OVR::Matrix4f proj, OVR::Matrix4f view);

        virtual void Resize(size_t width, size_t height) override;

        virtual void Shutdown() override;

    protected:

        void BindSampler(std::map<std::string, Texture*> textures, OmicronMaterial* mtl, std::string name, int id);

        void InitMirror(size_t width, size_t height);

        virtual void RenderFBO(std::vector<RenderCommand> cmds, ovrPosef* eyeRenderPoses);
        virtual void RenderMirror();

        ovrSessionStatus sessionStatus;

        ovrEyeRenderDesc eyeRenderDesc[2];
        OVR::Vector3f hmdToEyeViewOffset[2];
        ovrLayerEyeFov layer;
        ovrTextureSwapChain swapChain;

        ovr::TextureBuffer* eyeRenderTexture[2] = {nullptr, nullptr};
        ovr::DepthBuffer* eyeDepthBuffer[2] = {nullptr, nullptr};
        unsigned int mirrorFBO = 0;
        ovrMirrorTexture mirrorTexture = nullptr;

        ovr::TextureBuffer* fboRenderTexture[2] = {nullptr, nullptr};
        ovr::TextureBuffer* skyRenderTexture[2] = {nullptr, nullptr};
        ovr::TextureBuffer* fboLightRenderTexture[2] = {nullptr, nullptr};
        ovr::DepthBuffer* fboDepthBuffer[2] = {nullptr, nullptr};

        OVRContext* ovrContext; // pre-casted pointer to context
        bool readyForMirror = false;
    };

}

#endif //OMICRONRENDER_NEW_OVRRENDERER_HPP
