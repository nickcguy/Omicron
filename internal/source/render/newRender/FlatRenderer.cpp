//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/FlatRenderer.hpp>
#include <render/OpenGLContext.hpp>

namespace Omicron {

    FlatRenderer::FlatRenderer(OpenGLContext* context) : BaseRenderer(context) {}

    void FlatRenderer::Init() {

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("Default");
        defMtl->GetShader().CompileFromFile("assets/shaders/basic/basic.vert", "assets/shaders/basic/basic.frag");

        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("Quad");
        quadMtl->GetShader().CompileFromFile("assets/shaders/basic/quad.vert", "assets/shaders/basic/quad.frag");

        mtlManager.SetDefaultMaterial(defMtl);
        mtlManager.RegisterMaterial(quadMtl);

        mtlManager.RegisterMaterials();

        Resize(context->GetWidth(), context->GetHeight());
    }

    void FlatRenderer::Update(float delta) {
        mtlManager.Update(delta);
    }

    void FlatRenderer::Render(std::vector<RenderCommand> cmds) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_FRAMEBUFFER_SRGB);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        context->GetCamera()->updateCameraVectors();
        glm::mat4 view = context->GetCamera()->GetViewMatrix();

        for(RenderCommand cmd : cmds) {
            auto mtl = mtlManager.GetMaterialBase(cmd.material, true);
            mtl->SetUniforms(cmd.uniforms);
            mtl->GetShader().SetMatrix4("model", cmd.model);
            mtl->GetShader().SetInteger("outputbuffer", bufferType);
            mtl->GetShader().SetMatrix4("view", view);
            mtl->GetShader().SetMatrix4("projection", projection);
            BaseRenderer::Render(cmd);
        }
    }

    void FlatRenderer::Resize(size_t width, size_t height) {
        if(width == 0 || height == 0) return;
        float aspect = (float)width / (float)height;
        this->projection = glm::perspective(glm::radians(60.f), aspect, 0.1f, 1024.f);
    }

    void FlatRenderer::Shutdown() {

    }

}