//
// Created by Guy on 20/08/2017.
//

#ifndef OMICRONRENDER_DIRECTIONALSHADOWMAPPER_HPP
#define OMICRONRENDER_DIRECTIONALSHADOWMAPPER_HPP

#include <glm/glm.hpp>
#include <data/Light.hpp>
#include <render/RenderCommand.hpp>
#include <Extras/OVR_Math.h>
#include <render/ovr/TextureBuffer.hpp>

namespace Omicron {

    class BaseRenderer;

    class DirectionalShadowMapper {
    public:
        DirectionalShadowMapper(BaseRenderer* renderer, int size = 1024);
        void Init();
        void UpdateView(Light* light);
        void Render(std::vector<RenderCommand> solidCmds);

        unsigned int GetShadowMap(unsigned int positionBuffer);

        const glm::mat4& GetLightSpaceMatrix() const;

        unsigned int GetFBOMap();

    protected:
        unsigned int fbo;
        unsigned int map;

        ovr::TextureBuffer* buffer = nullptr;

        int size;
        BaseRenderer* renderer = nullptr;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 lightSpaceMatrix;
        Shader depthShader;
        Shader shadowShader;
    };

}

#endif //OMICRONRENDER_DIRECTIONALSHADOWMAPPER_HPP
