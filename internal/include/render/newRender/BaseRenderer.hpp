//
// Created by Guy on 14/08/2017.
//

#ifndef OMICRONRENDER_BASERENDERER_HPP
#define OMICRONRENDER_BASERENDERER_HPP

#include <pch.hpp>
#include <data/material/MaterialManager.hpp>
#include <render/RenderCommand.hpp>
#include <render/PrimitiveRenderer.hpp>
#include <Extras/OVR_Math.h>

namespace Omicron {

    class OpenGLContext;

    class BaseRenderer {
    public:
        BaseRenderer(OpenGLContext* context);
        virtual void Init()=0;
        virtual void Update(float delta);
        virtual void Render(std::vector<RenderCommand> cmds)=0;
        virtual void Render(RenderCommand cmd);
        virtual void Resize(size_t width, size_t height)=0;
        virtual void Shutdown()=0;
        virtual void RenderCubemap(OVR::Matrix4f proj, OVR::Matrix4f view);
        MaterialManager& GetMtlManager();

        void SetBufferType(int type);

        unsigned int GetPolygonMode() const;

        void SetPolygonMode(unsigned int polygonMode);

        void SetCubemap(std::vector<std::string> vector);
        void SetGradient(std::string imagePath);

    protected:
        unsigned int polygonMode = GL_FILL;
        MaterialManager mtlManager;
        OpenGLContext* context;
        PrimitiveRenderer primitiveRenderer;
        long long int frameIndex = 0;
        int bufferType = 0;
        Shader cubemapShader;
        float dayNightProgress = 0;
        float currentTime = 0;
        unsigned int cubemapId = 0;
        unsigned int dayNightGradientId = 0;
    };

}

#endif //OMICRONRENDER_BASERENDERER_HPP
