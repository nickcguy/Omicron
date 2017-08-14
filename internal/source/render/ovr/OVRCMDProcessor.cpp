//
// Created by Guy on 08/08/2017.
//

#include <render/ovr/OVRCMDProcessor.hpp>
#include <render/ovr/OVRRenderer.hpp>
#include <utils/TextUtils.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/MathsUtils.hpp>

namespace Omicron {

    namespace ovr {

        OVRCMDProcessor::OVRCMDProcessor(OVRRenderer* renderer) : renderer(renderer) {}

        void OVRCMDProcessor::Init(int width, int height) {
            leftFBO.Init();
            leftFBO.Bind();
            Utils::CheckErrors("LeftFBO - Init");
            auto leftAttachment = leftFBO.GenerateAttachmentTexture(GL_SRGB8_ALPHA8, GL_RGBA, width, height);
            Utils::CheckErrors("LeftFBO - GenerateAttachmentTexture");
            leftFBO.AddAttachment(leftAttachment);
            Utils::CheckErrors("LeftFBO - AddAttachment");
            leftFBO.InitRenderBuffer(width, height);
            Utils::CheckErrors("LeftFBO - InitRenderBuffer");

            rightFBO.Init();
            rightFBO.Bind();
            Utils::CheckErrors("RightFBO - Init");
            auto rightAttachment = rightFBO.GenerateAttachmentTexture(GL_SRGB8_ALPHA8, GL_RGBA, width, height);
            Utils::CheckErrors("RightFBO - GenerateAttachmentTexture");
            rightFBO.AddAttachment(rightAttachment);
            Utils::CheckErrors("RightFBO - AddAttachment");
            rightFBO.InitRenderBuffer(width, height);
            Utils::CheckErrors("RightFBO - InitRenderBuffer");


        }

        void OVRCMDProcessor::ResizeLeft(int width, int height) {
            leftFBO.Resize(width, height);
        }

        void OVRCMDProcessor::ResizeRight(int width, int height) {
            rightFBO.Resize(width, height);
        }

        void OVRCMDProcessor::SetLeftProjection(OVR::Matrix4f projection) {
            this->leftProj = projection;
        }

        void OVRCMDProcessor::SetLeftView(OVR::Matrix4f view) {
            this->leftView = view;
        }

        void OVRCMDProcessor::SetRightProjection(OVR::Matrix4f projection) {
            this->rightProj = projection;
        }

        void OVRCMDProcessor::SetRightView(OVR::Matrix4f view) {
            this->rightView = view;
        }

        unsigned int* OVRCMDProcessor::Render(std::vector<RenderCommand> cmds) {

            unsigned int* ids = new unsigned int[2];
            ids[0] = RenderLeft(cmds, leftProj, leftView);
            ids[1] = RenderRight(cmds, rightProj, rightView);

//            ids[0] = leftFBO.RBO().texId;
//            ids[1] = rightFBO.RBO().texId;

            return ids;

        }

        void OVRCMDProcessor::Render(RenderCommand cmd, OVR::Matrix4f projection, OVR::Matrix4f view) {
            auto mtl = renderer->GetMtlManager().GetMaterial(cmd.material, true);
            mtl->SetUniforms();
            renderer->PostIncludes(mtl);

            glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "view"), 1, GL_FALSE, (FLOAT*)&view.M);
            glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "projection"), 1, GL_FALSE, (FLOAT*)&projection.M);

//            glm::mat4 projMat = Utils::ConvertMat4(projection);
//            glm::mat4 viewMat = Utils::ConvertMat4(view);

//            mtl->GetShader().SetMatrix4("projection", projMat);
//            mtl->GetShader().SetMatrix4("view", viewMat);
            mtl->GetShader().SetMatrix4("model", cmd.model);
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

        unsigned int OVRCMDProcessor::RenderLeft(std::vector<RenderCommand> cmds, OVR::Matrix4f projection, OVR::Matrix4f view) {
            leftFBO.Bind();
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, leftFBO.width, leftFBO.height);

            for(const RenderCommand& cmd : cmds)
                Render(cmd, projection, view);

            leftFBO.Unbind();

            return leftFBO.GetAttachment(0).texId;
        }

        unsigned int OVRCMDProcessor::RenderRight(vector<RenderCommand> cmds, Matrix4f projection, Matrix4f view) {
            rightFBO.Bind();
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, rightFBO.width, rightFBO.height);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            glDepthFunc(GL_ALWAYS);
            for(const RenderCommand& cmd : cmds) {
                Render(cmd, projection, view);
            }

            rightFBO.Unbind();

            return rightFBO.GetAttachment(0).texId;
        }

    }

}