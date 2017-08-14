//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_OVRRENDERER_HPP
#define OMICRONRENDER_OVRRENDERER_HPP

#include <render/OpenGLRenderer.hpp>
#include <OVR_CAPI.h>
#include <Extras/OVR_Math.h>
#include <engine/entity/OmicronEntity.hpp>
#include <render/PrimitiveRenderer.hpp>
#include "TextureBuffer.hpp"
#include "OVRCMDProcessor.hpp"

namespace Omicron {

    namespace ovr {

        class OVRRenderer : public OpenGLRenderer {
        public:
            OVRRenderer(OpenGLContext* context);

            virtual void PreRender(float delta) override;
            virtual void Init() override;
            virtual void Shutdown() override;

            void UpdateTracking();

            virtual bool HandlesOwnLoop() override;

            virtual void Render(RenderCommand cmd) override;

            virtual void Render(RenderCommand cmd, OVR::Matrix4f& proj, OVR::Matrix4f& view, bool toDeferred = true);
            virtual void Render(std::vector<RenderCommand> cmd) override;

            void SetHandEntity(ovrHandType hand, OmicronEntity* entity);

            OVR::Posef VirtualWorldTransformfromRealPose(const Posef &sensorHeadPose, ovrTrackingOrigin trackingOrigin);

        protected:

            void RenderFBO(std::vector<RenderCommand> cmds, ovrPosef EyeRenderPose[2]);
            void RenderDirect(std::vector<RenderCommand> cmds, ovrPosef EyeRenderPose[2]);

            ovrInitParams params;
            ovrSession session;
            ovrGraphicsLuid luid;
            ovrEyeRenderDesc eyeRenderDesc[2];
            ovrVector3f hmdToEyeViewOffset[2];
            ovrLayerEyeFov layer;
            ovrTrackingState trackingState;
            OVR::Posef headPose;
            ovrHmdDesc desc;
            ovrTextureSwapChain swapChain = 0;
            OVR::Matrix4f projections[2];
            int currentTextureIndex = 0;
            OVRCMDProcessor cmdProcessor;
            PrimitiveRenderer primitiveRenderer;

            TextureBuffer* eyeRenderTexture[2] = {nullptr, nullptr};
            DepthBuffer* eyeDepthBuffer[2] = {nullptr, nullptr};
            unsigned int mirrorFBO;
            ovrMirrorTexture mirrorTexture = nullptr;

            OmicronEntity* handEntities[2] = {nullptr, nullptr};

            TextureBuffer* fboRenderTexture[2] = {nullptr, nullptr};
            TextureBuffer* fboAlphaRenderTexture[2] = {nullptr, nullptr};
            DepthBuffer* fboDepthBuffer[2] = {nullptr, nullptr};

            bool useFBO = true;
        };

    }

}

#endif //OMICRONRENDER_OVRRENDERER_HPP
