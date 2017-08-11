//
// Created by Guy on 06/08/2017.
//

#include <render/OpenGLRenderer.hpp>
#include <vector>
#include <render/RenderCommand.hpp>
#include <algorithm>
#include <render/OpenGLContext.hpp>
#include <sstream>
#include <utils/TextUtils.hpp>

namespace Omicron {

    OpenGLRenderer::OpenGLRenderer(OpenGLContext* context) : matrixBuffer(GL_STATIC_DRAW, "Matrices"), lightBuffer(GL_STATIC_DRAW, "LightBuffer"), context(context) {}

    void OpenGLRenderer::Init() {
        shader.CompileFromFile("assets/shaders/basic/basic.vert", "assets/shaders/basic/basic.frag");

        mtlManager.SetOnNewMaterial([=](OmicronMaterial* mtl) {
            mtl->InvalidateSamplers();
            mtl->GetShader().CompileFromSources();
            mtl->SetReady(true);
        });

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("Default");
        defMtl->SetShader(shader);

        Shader quadShader;
        quadShader.CompileFromFile("assets/shaders/basic/quad.vert", "assets/shaders/basic/quad.frag");
        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("Textured Screen Quad");
        quadMtl->SetShader(shader);

        mtlManager.RegisterMaterial(quadMtl);
        Utils::CheckErrors("RegisterMaterial");

        mtlManager.SetDefaultMaterial(defMtl);
        Utils::CheckErrors("SetDefaultMaterial");

        lightBuffer.Allocate();
        Utils::CheckErrors("Allocate");

        mtlManager.RegisterMaterials();
        Utils::CheckErrors("RegisterMaterials");
        lightData.lights.push_back(Light{ { 0.0f, 0.0f, 10.0f }, {150.f, 150.f, 150.f} });

    }

    void OpenGLRenderer::SetProjection(glm::mat4 projection) {
        matrices.projection = projection;
    }

    void OpenGLRenderer::SetView(glm::mat4 view) {
        matrices.view = view;
    }

    void OpenGLRenderer::PreRender(float delta) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_PROGRAM_POINT_SIZE);

        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        SSBOLightData data = lightData.Flatten();
        lightBuffer.SetData(sizeof(data), (void*)&data);

        size_t mat4Size = sizeof(glm::mat4);

//        matrixBuffer.SetUniformMatrix(matrices.projection, mat4Size, 0);
//        matrixBuffer.SetUniformMatrix(matrices.view, mat4Size, mat4Size);
        mtlManager.Update(delta);
    }

    std::vector<RenderCommand> OpenGLRenderer::Sort(std::vector<RenderCommand> cmds) {
        std::sort(cmds.begin(), cmds.end(), [this](const RenderCommand& a, const RenderCommand& b) -> bool {
            return a.alpha < b.alpha;
            int aOrd = static_cast<int>(a.material.length());
            int bOrd = static_cast<int>(b.material.length());
            return aOrd < bOrd;
        });
        return cmds;
    }

    void OpenGLRenderer::Render(RenderCommand cmd) {
        auto mtl = mtlManager.GetMaterial(cmd.material, cmd.materialInstance, true);
        mtl->SetUniforms(cmd.uniforms);
        PostIncludes(mtl);
        mtl->GetShader().SetMatrix4("model", cmd.model);
        mtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));
        switch(cmd.type) {
            case VERTEX: case POINTCLOUD:
                glBindVertexArray(cmd.VAO);
                glDrawArrays(cmd.primitive, cmd.offset, cmd.size);
                break;
            case INDEXED:
                glBindVertexArray(cmd.VAO);
                glDrawElements(cmd.primitive, cmd.size, cmd.indexType, (void*) cmd.offset);
                break;
            default: break;
        }
    }

    void OpenGLRenderer::PostRender() {
        glBindVertexArray(0);
        frameIndex++;
    }

    void OpenGLRenderer::SetBufferType(BufferType type) {
        this->bufferType = type;
    }

    void OpenGLRenderer::SetPolygonMode(unsigned int polygonMode) {
        this->polygonMode = polygonMode;
    }

    unsigned int OpenGLRenderer::GetPolygonMode() {
        return polygonMode;
    }

    void OpenGLRenderer::PostIncludes(OmicronMaterial* mtl) {

        mtl->GetShader().SetMatrix4("projection", matrices.projection);
        mtl->GetShader().SetMatrix4("view", matrices.view);

        for(std::string include : mtl->GetIncludes()) {
            if(include == "Camera") {
                mtl->GetShader().SetVector3f("camPos", context->GetCamera().Position);
                mtl->GetShader().SetVector3f("camDir", context->GetCamera().Front);
                return;
            }

            if(include == "Lighting") {
//                lightBuffer.Map();
                for(int i = 0; i < lightData.lights.size(); i++) {
                    Light light = lightData.lights[i];

                    std::stringstream ss;
                    ss << "[" << i << "]";
                    std::string suffix = ss.str();

                    mtl->GetShader().SetVector3f(("lightPositions" + suffix).c_str(), light.position);
                    mtl->GetShader().SetVector3f(("lightColours" + suffix).c_str(), light.colour);
                }

                return;
            }

            if(include == "Buffer") {
                mtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));
                return;
            }
        }
    }

    void OpenGLRenderer::Shutdown() {

    }

    void OpenGLRenderer::Resize(size_t width, size_t height) {
        SetProjection(glm::perspective(45.f, (float)width / (float)height, 0.1f, 1024.f));
        this->windowWidth = width;
        this->windowHeight = height;
    }

    bool OpenGLRenderer::HandlesOwnLoop() {
        return false;
    }

    void OpenGLRenderer::Render(std::vector<RenderCommand> cmd) {}

    MaterialManager& OpenGLRenderer::GetMtlManager() {
        return mtlManager;
    }

    glm::vec3 OpenGLRenderer::GetCameraPosition() {
        return context->GetCamera().Position;
    }


};