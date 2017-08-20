//
// Created by Guy on 20/08/2017.
//

#include <render/DecalRenderer.hpp>

namespace Omicron {

    DecalRenderer::DecalRenderer(int width, int height) : width(width), height(height) {
        decalBuffer = new ovr::TextureBuffer(nullptr, true, false, OVR::Sizei(width, height), 1, NULL, 1, 5);
        decalShader.CompileFromFile("assets/shaders/ovr/decal.vert", "assets/shaders/ovr/decal.frag");
    }

    void DecalRenderer::RenderDecals(std::vector<RenderCommand> cmds, OVR::Matrix4f projection, OVR::Matrix4f view, ovr::TextureBuffer* textureBuffer, ovr::DepthBuffer* depthBuffer) {
        PopulateDecalBuffer(textureBuffer);
        textureBuffer->SetAndClearRenderSurface(depthBuffer, false, false);
        glDisable(GL_DEPTH_TEST);
        decalShader.Use();
        glUniformMatrix4fv(decalShader.Find("projection"), 1, GL_FALSE, (FLOAT*)&projection);
        glUniformMatrix4fv(decalShader.Find("view"),       1, GL_FALSE, (FLOAT*)&view);
        for(const auto& cmd : cmds)
            RenderDecal(cmd, decalBuffer, depthBuffer);
        glEnable(GL_DEPTH_TEST);
        textureBuffer->UnsetRenderSurface();
    }

    void DecalRenderer::RenderDecal(RenderCommand cmd, ovr::TextureBuffer* sampleBuffer, ovr::DepthBuffer* depthBuffer) {

        int max = sampleBuffer->texIds.size();
        for(int i = 0; i < max; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, sampleBuffer->texIds[i]);
            std::stringstream ss;
            ss << "bufferTex_" << i;
            decalShader.SetInteger(ss.str().c_str(), i);
        }

        if(depthBuffer) {
            glActiveTexture(GL_TEXTURE0 + max);
            glBindTexture(GL_TEXTURE_2D, depthBuffer->texId);
            decalShader.SetInteger("DepthBuffer", max);
        }

        decalShader.SetMatrix4("model", cmd.model);
        RenderDecalCube();
    }

    void DecalRenderer::PopulateDecalBuffer(ovr::TextureBuffer* buffer) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer->fboId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, decalBuffer->fboId);
        int w = width;
        int h = height;
        glBlitFramebuffer(0, h, w, 0,
                          0, 0, w, h,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    void DecalRenderer::RenderDecalCube() {
        if(decalCubeVAO <= 0) {

            float cubeVertices[] = {
            // positions
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
            };

            glGenVertexArrays(1, &decalCubeVAO);
            glGenBuffers(1, &decalCubeVBO);
            glBindVertexArray(decalCubeVAO);
            glBindBuffer(GL_ARRAY_BUFFER, decalCubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        }else
            glBindVertexArray(decalCubeVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

}