//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/BaseRenderer.hpp>
#include <render/Cubemap.hpp>
#include <Extras/OVR_Math.h>
#include <stb_image.h>
#include <render/OpenGLContext.hpp>

namespace Omicron {

    BaseRenderer::BaseRenderer(OpenGLContext* context) : context(context), shadowMapper(this, 1024) {
        cubemapShader.CompileFromFile("assets/shaders/basic/Cubemap.vert", "assets/shaders/basic/Cubemap.frag");
    }

    void BaseRenderer::Render(RenderCommand cmd) {
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

    MaterialManager& BaseRenderer::GetMtlManager() {
        return mtlManager;
    }

    void BaseRenderer::SetBufferType(int type) {
        this->bufferType = type;
    }

    unsigned int BaseRenderer::GetPolygonMode() const {
        return polygonMode;
    }

    void BaseRenderer::SetPolygonMode(unsigned int polygonMode) {
        BaseRenderer::polygonMode = polygonMode;
    }

    void BaseRenderer::RenderCubemap(OVR::Matrix4f proj, OVR::Matrix4f view) {
        if(cubemapId == 0) return;

        view = OVR::Matrix4f(OVR::Matrix3f(view.GetXBasis(), view.GetYBasis(), view.GetZBasis()));

        cubemapShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(cubemapShader.ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
        glUniformMatrix4fv(glGetUniformLocation(cubemapShader.ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dayNightGradientId);
        cubemapShader.SetInteger("gradient", 1);
        cubemapShader.SetFloat("dayNight", dayNightProgress);
//        cubemapShader.SetMatrix4("projection", proj);
//        cubemapShader.SetMatrix4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
        primitiveRenderer.RenderSkybox();

    }

    void BaseRenderer::SetCubemap(std::vector<std::string> faces) {
        cubemapId = Cubemap::LoadCubemap(faces);
    }

    void BaseRenderer::Update(float delta) {
        mtlManager.Update(delta);
        currentTime += delta;
        dayNightProgress = glm::sin((currentTime / 120) / 120);
    }

    void BaseRenderer::SetGradient(std::string imagePath) {
        glGenTextures(1, &dayNightGradientId);
        glBindTexture(GL_TEXTURE_2D, dayNightGradientId);
        int width, height, nrChannels;
            unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
        if(data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }else{
            printf("Cubemap gradient texture failed to load, \"%s\"\n", imagePath.c_str());
        }
        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}