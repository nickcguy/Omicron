//
// Created by Guy on 20/08/2017.
//

#include <render/shadow/DirectionalShadowMapper.hpp>
#include <pch.hpp>
#include <render/OpenGLContext.hpp>
#include <render/newRender/BaseRenderer.hpp>

namespace Omicron {

    DirectionalShadowMapper::DirectionalShadowMapper(BaseRenderer* renderer, int size) : renderer(renderer), size(size) {}

    void DirectionalShadowMapper::Init() {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &map);
        glBindTexture(GL_TEXTURE_2D, map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("Incomplete framebuffer");
            Utils::CheckErrors("Framebuffer");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        float nearPlane = 0.1f;
        float farPlane  = 1024.f;

        float halfSize = size / 2.f;

        projection = glm::ortho(-halfSize, halfSize, -halfSize, halfSize, nearPlane, farPlane);

        depthShader.CompileFromFile("assets/shaders/advanced/shadow/Depth.vert", "assets/shaders/advanced/shadow/Depth.frag");

        buffer = new ovr::TextureBuffer(nullptr, true, false, OVR::Sizei(renderer->context->GetWidth(), renderer->context->GetHeight()), 1, NULL, 1);

        shadowShader.CompileFromFile("assets/shaders/advanced/shadow/Shadow.vert", "assets/shaders/advanced/shadow/Shadow.frag");
    }

    void DirectionalShadowMapper::UpdateView(Light* light) {
        view = glm::lookAt(light->position, light->position + light->direction, glm::vec3(0.f, 1.f, 0.f));
    }

    void DirectionalShadowMapper::Render(std::vector<RenderCommand> solidCmds) {
        glViewport(0, 0, size, size);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        depthShader.Use();
        lightSpaceMatrix = projection * view;
        depthShader.SetMatrix4("lightSpace", lightSpaceMatrix);
        for(RenderCommand cmd : solidCmds) {
            depthShader.SetMatrix4("model", cmd.model);
            glBindVertexArray(cmd.VAO);
            switch(cmd.type) {
                case VERTEX: case POINTCLOUD:
                    glDrawArrays(cmd.primitive, cmd.offset, cmd.size);
                    break;
                case INDEXED:
                    glDrawElements(cmd.primitive, cmd.size, cmd.indexType, (void*) cmd.offset);
                    break;
                default: break;
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, renderer->context->GetWidth(), renderer->context->GetHeight());
    }

    const glm::mat4& DirectionalShadowMapper::GetLightSpaceMatrix() const {
        return lightSpaceMatrix;
    }

    unsigned int DirectionalShadowMapper::GetShadowMap(unsigned int positionBuffer) {
        buffer->SetAndClearRenderSurface(nullptr, false, false, false);
        Utils::CheckErrors("glDisable(GL_CULL_FACE)");
        shadowShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, positionBuffer);
        Utils::CheckErrors("glBindTexture(GL_TEXTURE_2D, positionBuffer)");
        shadowShader.SetInteger("shadowMap", 0);
        shadowShader.SetInteger("positionBuffer", 1);
        shadowShader.SetMatrix4("lightSpace", lightSpaceMatrix);
        renderer->primitiveRenderer.RenderQuad();
        Utils::CheckErrors("renderer->primitiveRenderer.RenderQuad()");

        buffer->UnsetRenderSurface();

        return buffer->texIds[0];
    }

    unsigned int DirectionalShadowMapper::GetFBOMap() {
        return map;
    }

}