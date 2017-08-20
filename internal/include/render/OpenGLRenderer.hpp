//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OPENGLRENDERR_HPP
#define OMICRONRENDER_OPENGLRENDERR_HPP

#include <render/shader/Shader.hpp>
#include <vector>
#include <data/UBO.hpp>
#include <data/SSBO.hpp>
#include <data/material/MaterialManager.hpp>
#include "RenderCommand.hpp"

namespace Omicron {

    class OpenGLContext;

    struct Matrices {
        glm::mat4 projection;
        glm::mat4 view;
    };

    struct SSBOLightData {
        int lightCount;
        int lightStride;
        int lightPositionOffset;
        int lightColourOffset;
        float* lightData;
    };

    struct Light2 {
        glm::vec3 position;
        glm::vec3 colour;
    };

    struct LightData {
        std::vector<Light2> lights;

        inline SSBOLightData Flatten() {
            SSBOLightData data = {};
            data.lightCount = lights.size();
            data.lightStride = 6;
            data.lightPositionOffset = 0;
            data.lightColourOffset = 3;

            std::vector<float> lightData(data.lightCount * data.lightStride);
            int i = 0;
            for(Light2 light : lights) {
                lightData[i++] = light.position.x;
                lightData[i++] = light.position.y;
                lightData[i++] = light.position.z;

                lightData[i++] = light.colour.x;
                lightData[i++] = light.colour.y;
                lightData[i++] = light.colour.z;
            }
            data.lightData = lightData.data();
            return data;
        }
    };

    enum BufferType {
        BUF_COLOUR,
        BUF_NORMAL,
        BUF_WORLD_NORMAL,
        BUF_TEXCOORDS,
        BUF_POSITION,
        BUF_5,
        BUF_6,
        BUF_7,
        BufferType_LENGTH
    };

    class OpenGLRenderer {
    public:

        OpenGLRenderer(OpenGLContext* context);

        virtual void Init();

        void SetProjection(glm::mat4 projection);
        void SetView(glm::mat4 view);

        virtual void PreRender(float delta);
        virtual std::vector <RenderCommand> Sort(std::vector<RenderCommand> cmds);
        virtual void PostIncludes(OmicronMaterial* mtl);
        virtual void Render(RenderCommand cmd);
        virtual void Render(std::vector<RenderCommand> cmd);
        virtual void PostRender();
        virtual void Resize(size_t width, size_t height);

        void SetBufferType(BufferType type);
        void SetPolygonMode(unsigned int polygonMode);
        unsigned int GetPolygonMode();

        virtual void Shutdown();

        virtual bool HandlesOwnLoop();

        MaterialManager& GetMtlManager();

        glm::vec3 GetCameraPosition();

        OpenGLContext* GetContext() const;

    protected:
        unsigned int polygonMode = GL_FILL;
        Shader shader;

        Matrices matrices;
        UBO matrixBuffer;
        SSBO lightBuffer;
        LightData lightData;
        MaterialManager mtlManager;
        OpenGLContext* context;

        BufferType bufferType = BufferType::BUF_COLOUR;
        long long int frameIndex = 0;

        size_t windowWidth;
        size_t windowHeight;
    };

};

#endif //OMICRONRENDER_OPENGLRENDERR_HPP
