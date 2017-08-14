//
// Created by Guy on 14/08/2017.
//

#ifndef OMICRONRENDER_FLATRENDERER_HPP
#define OMICRONRENDER_FLATRENDERER_HPP

#include "BaseRenderer.hpp"

namespace Omicron {

    class FlatRenderer : public BaseRenderer {
    public:
        FlatRenderer(OpenGLContext* context);

        virtual void Init() override;

        virtual void Update(float delta) override;

        virtual void Render(std::vector<RenderCommand> cmds) override;

        virtual void Resize(size_t width, size_t height) override;

        virtual void Shutdown() override;

    protected:
        glm::mat4 projection;
    };

}

#endif //OMICRONRENDER_FLATRENDERER_HPP
