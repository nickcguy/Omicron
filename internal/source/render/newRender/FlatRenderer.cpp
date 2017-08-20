//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/FlatRenderer.hpp>
#include <render/OpenGLContext.hpp>

namespace Omicron {

    FlatRenderer::FlatRenderer(OpenGLContext* context) : BaseRenderer(context) {}

    void FlatRenderer::Init() {

        mtlManager.SetOnNewMaterial([=](OmicronMaterial* mtl) {
            printf("New material registered: %s\n", mtl->GetName().c_str());
            mtl->InvalidateSamplers();
            mtl->GetShader().CompileFromSources();
            mtl->SetReady(true);
        });

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("Default");
        defMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/basic.vert", "assets/shaders/ovr/deferred_pbr/basic.frag");
        defMtl->SetReady(true);

        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("Quad");
        quadMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/quad.vert", "assets/shaders/ovr/deferred_pbr/quad.frag");
        quadMtl->SetReady(true);

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

        std::vector<RenderCommand> solidCmds;
        std::vector<RenderCommand> alphaCmds;

        for(RenderCommand cmd : cmds) {
            if(cmd.alpha == 1.0)
                solidCmds.push_back(cmd);
            else alphaCmds.push_back(cmd);
        }

        auto deferredMtl = mtlManager.GetMaterialBase("Deferred", true);

//        glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
//        glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);

        deferredMtl->GetShader().Use();

        deferredMtl->GetShader().SetMatrix4("view", view);
        deferredMtl->GetShader().SetMatrix4("projection", projection);
        deferredMtl->GetShader().SetInteger("outputbuffer", bufferType);

        fboRenderTexture->SetAndClearRenderSurface(fboDepthTexture);
        for(RenderCommand cmd : solidCmds)
            Render(cmd, projection, view);
        glDepthMask(GL_FALSE);
        for(RenderCommand cmd : alphaCmds)
            Render(cmd, projection, view);
        glDepthMask(GL_TRUE);
        fboRenderTexture->UnsetRenderSurface();
//        screenRenderTexture->SetAndClearRenderSurface(nullptr);
        auto quadMtl = mtlManager.GetMaterial("Quad", true);
        quadMtl->GetShader().Use();
        quadMtl->GetShader().SetInteger("outputBuffer", bufferType);
        glDisable(GL_CULL_FACE);
        const char* uniformNames[] = {
        "AlbedoSpec",
        "Normal",
        "Metallic",
        "Roughness",
        "AO",
        "Position"
        };

        for(int j = 0; j < 6; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, fboRenderTexture->texIds[j]);
            quadMtl->GetShader().SetInteger(uniformNames[j], j);
        }
        quadMtl->GetShader().SetVector3f("viewPos", context->GetCamera()->Position);
        primitiveRenderer.RenderQuad();
        glEnable(GL_CULL_FACE);
//        screenRenderTexture->UnsetRenderSurface();

//        BlitToScreen();

    }

    void FlatRenderer::Resize(size_t width, size_t height) {
        if(width == 0 || height == 0) return;
        float aspect = (float)width / (float)height;
        this->projection = glm::perspective(glm::radians(60.f), aspect, 0.1f, 1024.f);

        if(fboRenderTexture) {
            CLEAR_PTR(fboRenderTexture);
        }

        if(fboDepthTexture) {
            CLEAR_PTR(fboDepthTexture);
        }

        if(screenRenderTexture) {
            CLEAR_PTR(screenRenderTexture);
        }

        fboRenderTexture = new ovr::TextureBuffer(nullptr, true, false, OVR::Sizei(width, height), 1, NULL, 1, 6);
        fboDepthTexture = new ovr::DepthBuffer(fboRenderTexture->GetSize(), 1);
        screenRenderTexture = new ovr::TextureBuffer(nullptr, true, false, OVR::Sizei(width, height), 1, NULL, 1);
    }

    void FlatRenderer::Shutdown() {
        CLEAR_PTR(fboRenderTexture);
        CLEAR_PTR(fboDepthTexture);
    }

    void FlatRenderer::BlitToScreen(int targetFBO) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, screenRenderTexture->fboId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO);
        int w = context->GetWidth();
        int h = context->GetHeight();
        glBlitFramebuffer(0, 0, w, h,
                          0, 0, w, h,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    void FlatRenderer::Render(RenderCommand cmd, glm::mat4 proj, glm::mat4 view) {
        auto mtl = mtlManager.GetMaterial(cmd.material, cmd.materialInstance, true);

        auto deferredMtl = mtlManager.GetMaterialBase("Default", true);
        auto samplers = mtl->GetSamplers();
        deferredMtl->SetUniforms();
//            PostIncludes(deferredMtl);

        int index = 0;
        for(auto pair : samplers) {
            glActiveTexture(GL_TEXTURE0 + index);
            pair.second->Bind();
            deferredMtl->GetShader().SetInteger((pair.first + "Map").c_str(), index);
            index++;
        }

//        glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
//        glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);
        deferredMtl->GetShader().SetMatrix4("projection", projection);
        deferredMtl->GetShader().SetMatrix4("view", view);
        deferredMtl->GetShader().SetMatrix4("model", cmd.model);
        deferredMtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));

        BaseRenderer::Render(cmd);
    }

}