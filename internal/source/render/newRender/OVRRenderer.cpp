//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/OVRRenderer.hpp>
#include <render/OpenGLContext.hpp>
#include <render/ovr/OVRContext.hpp>
#include <utils/TextUtils.hpp>
#include <utils/MathsUtils.hpp>

namespace Omicron {

    OVRRenderer::OVRRenderer(OpenGLContext* context) : BaseRenderer(context) {}

    void OVRRenderer::Init() {

        mtlManager.SetOnNewMaterial([=](OmicronMaterial* mtl) {
            printf("New material registered: %s\n", mtl->GetName().c_str());
            mtl->InvalidateSamplers();
            mtl->GetShader().CompileFromSources();
            mtl->SetReady(true);
        });

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("PBRDefault");
        defMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/basic.vert", "assets/shaders/ovr/deferred_pbr/basic.frag");
        defMtl->SetReady(true);

        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("PBRQuad");
        quadMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/quad.vert", "assets/shaders/ovr/deferred_pbr/quad.frag");
        quadMtl->SetReady(true);

        mtlManager.SetDefaultMaterial(defMtl);
        mtlManager.RegisterMaterial(quadMtl);

        mtlManager.RegisterMaterials();

        Resize(context->GetWidth(), context->GetHeight());

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void OVRRenderer::Update(float delta) {
        mtlManager.Update(delta);
        Utils::CheckErrors("mtlManager.Update(delta)");
    }

    void OVRRenderer::Render(std::vector<RenderCommand> cmds) {
        ovr_GetSessionStatus(ovrContext->GetSession(), &sessionStatus);
        if(sessionStatus.ShouldQuit) {
            glfwSetWindowShouldClose(context->GetWindow(), true);
            return;
        }
        if(sessionStatus.ShouldRecenter)
            ovr_RecenterTrackingOrigin(ovrContext->GetSession());

        if(sessionStatus.IsVisible) {
            ovrEyeRenderDesc eyeRenderDesc[2];
            ovrPosef eyeRenderPose[2];
            ovrVector3f hmdToEyeOffset[2];
            for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
                eyeRenderDesc[i] = ovr_GetRenderDesc(ovrContext->GetSession(), static_cast<ovrEyeType>(i), ovrContext->GetHmdDesc().DefaultEyeFov[i]);
                hmdToEyeOffset[i] = eyeRenderDesc[i].HmdToEyeOffset;
            }
            double sensorSampleTime = 0;
            ovr_GetEyePoses(ovrContext->GetSession(), frameIndex, ovrTrue, hmdToEyeOffset, eyeRenderPose, &sensorSampleTime);

            Utils::CheckErrors("Pre Render FBO");
            RenderFBO(cmds, eyeRenderPose);
            Utils::CheckErrors("Render FBO");


            ovrLayerEyeFov ld;
            ld.Header.Type = ovrLayerType_EyeFov;
            ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

            for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
                ld.ColorTexture[i] = eyeRenderTexture[i]->TextureChain;
                ld.Viewport[i] = OVR::Recti(eyeRenderTexture[i]->GetSize());
                ld.Fov[i] = ovrContext->GetHmdDesc().DefaultEyeFov[i];
                ld.RenderPose[i] = eyeRenderPose[i];
            }
            ld.SensorSampleTime = sensorSampleTime;

            const ovrLayerHeader* layers = &ld.Header;
            ovr_SubmitFrame(ovrContext->GetSession(), frameIndex, nullptr, &layers, 1);
            Utils::CheckErrors("ovr_SubmitFrame");

            frameIndex++;
        }
        Utils::CheckErrors("Pre mirror render");
        RenderMirror();
        Utils::CheckErrors("Post mirror render");
    }

    void OVRRenderer::Resize(size_t width, size_t height) {

    }

    void OVRRenderer::Shutdown() {

    }

    void OVRRenderer::InitOVR() {
        if(OVRContext* context = dynamic_cast<OVRContext*>(this->context)) {
            ovrContext = context;

            auto session = context->GetSession();
            auto hmdDesc = context->GetHmdDesc();
            float pixelDensity = 1.0f;

            OVR::Sizei tex0Size = ovr_GetFovTextureSize(session, ovrEye_Left, hmdDesc.DefaultEyeFov[ovrEye_Left],
                                                        pixelDensity);
            OVR::Sizei tex1Size = ovr_GetFovTextureSize(session, ovrEye_Right, hmdDesc.DefaultEyeFov[ovrEye_Right],
                                                        pixelDensity);

            OVR::Sizei bufferSize;
            bufferSize.w = tex0Size.w + tex1Size.w;
            bufferSize.h = std::max(tex0Size.w, tex1Size.w);

            ovrTextureSwapChainDesc swapChainDesc = {};
            swapChainDesc.Type = ovrTexture_2D;
            swapChainDesc.ArraySize = 1;
            swapChainDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
            swapChainDesc.Width = bufferSize.w;
            swapChainDesc.Height = bufferSize.h;
            swapChainDesc.MipLevels = 0;
            swapChainDesc.SampleCount = 1;
            swapChainDesc.StaticImage = ovrFalse;

            if(ovr_CreateTextureSwapChainGL(session, &swapChainDesc, &swapChain) == ovrSuccess) {
                unsigned int texId;
                ovr_GetTextureSwapChainBufferGL(session, swapChain, 0, &texId);
                glBindTexture(GL_TEXTURE_2D, texId);
            }

            Utils::CheckErrors("Swapchain");

            for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
                ovrEyeType eye = static_cast<ovrEyeType>(i);
                eyeRenderDesc[i] = ovr_GetRenderDesc(session, eye, hmdDesc.DefaultEyeFov[i]);
                hmdToEyeViewOffset[i] = eyeRenderDesc[i].HmdToEyeOffset;
                layer.ColorTexture[i] = swapChain;
                layer.Fov[i] = eyeRenderDesc[i].Fov;

                OVR::Sizei idealTexSize = ovr_GetFovTextureSize(session, eyeRenderDesc[i].Eye, hmdDesc.DefaultEyeFov[i], 1);
                eyeRenderTexture[i] = new ovr::TextureBuffer(session, true, true, idealTexSize, 1, NULL, 1);
                eyeDepthBuffer[i] = new ovr::DepthBuffer(eyeRenderTexture[i]->GetSize(), 0);
                fboRenderTexture[i] = new ovr::TextureBuffer(session, true, false, idealTexSize, 1, NULL, 1, 4);
                fboDepthBuffer[i] = new ovr::DepthBuffer(fboRenderTexture[i]->GetSize(), 0);
            }

            Utils::CheckErrors("FBO");

            layer.Viewport[ovrEye_Left]  = OVR::Recti(0, 0,                bufferSize.w / 2, bufferSize.h);
            layer.Viewport[ovrEye_Right] = OVR::Recti(bufferSize.w / 2, 0, bufferSize.w / 2, bufferSize.h);

            ovrMirrorTextureDesc mirrorDesc = {};
            memset(&mirrorDesc, 0, sizeof(mirrorDesc));
            mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
            mirrorDesc.Width = context->GetWidth();
            mirrorDesc.Height = context->GetHeight();
            ovr_CreateMirrorTextureGL(session, &mirrorDesc, &mirrorTexture);

            unsigned int texId;
            ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId);
            glGenFramebuffers(1, &mirrorFBO);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
            glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            Utils::CheckErrors("Mirror");

        }else throw std::runtime_error("OVRRenderer needs to be paired to an OVRContext");

    }

    void OVRRenderer::RenderFBO(std::vector<RenderCommand> cmds, ovrPosef* eyeRenderPoses) {
        OVR::Vector3f Pos = {
        context->GetCamera()->Position.x,
        context->GetCamera()->Position.y,
        context->GetCamera()->Position.z,
        };

        Pos.x += ovrContext->worldPosition.x;
        Pos.y += ovrContext->worldPosition.y;
        Pos.z += ovrContext->worldPosition.z;

        static float Yaw(0.f);

        std::vector<RenderCommand> solidCmds;
        std::vector<RenderCommand> alphaCmds;

        for(RenderCommand cmd : cmds) {
            if(cmd.alpha == 1.0)
                solidCmds.push_back(cmd);
            else alphaCmds.push_back(cmd);
        }

        OVR::Vector3f shiftedEyePositions[2];

        for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {

            OVR::Matrix4f rollPitchYaw = OVR::Matrix4f::RotationY(Yaw);
            OVR::Matrix4f finalRollPitchYaw = rollPitchYaw * OVR::Matrix4f(eyeRenderPoses[i].Orientation);
            OVR::Vector3f finalUp = finalRollPitchYaw.Transform(OVR::Vector3f(0, 1, 0));
            OVR::Vector3f finalForward = finalRollPitchYaw.Transform(OVR::Vector3f(0, 0, -1));
            OVR::Vector3f shiftedEyePos = Pos + rollPitchYaw.Transform(eyeRenderPoses[i].Position);
            shiftedEyePositions[i] = shiftedEyePos;

            OVR::Matrix4f proj = ovrMatrix4f_Projection(ovrContext->GetHmdDesc().DefaultEyeFov[i], 0.1f, 1000.f, ovrProjection_None);
            OVR::Matrix4f view = OVR::Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);


            Utils::CheckErrors("Pre fboRenderTexture[i]->SetAndClearRenderSurface(fboDepthBuffer[i])");
            fboRenderTexture[i]->SetAndClearRenderSurface(fboDepthBuffer[i]);
            Utils::CheckErrors("fboRenderTexture[i]->SetAndClearRenderSurface(fboDepthBuffer[i])");
            // TODO apply proj/view matrices to render calls
            for(RenderCommand cmd : solidCmds)
                Render(cmd, proj, view);
            Utils::CheckErrors("for(RenderCommand cmd : solidCmds) Render(cmd, proj, view)");
            glDepthMask(GL_FALSE);
            Utils::CheckErrors("glDepthMask(GL_FALSE)");
            glBlendFunc(GL_ONE, GL_ONE);
            Utils::CheckErrors("glBlendFunc(GL_ONE, GL_ONE)");
            for(RenderCommand cmd : alphaCmds)
                Render(cmd, proj, view);
            Utils::CheckErrors("for(RenderCommand cmd : alphaCmds) Render(cmd, proj, view)");
            glDepthMask(GL_TRUE);
            Utils::CheckErrors("glDepthMask(GL_TRUE)");
            fboRenderTexture[i]->UnsetRenderSurface();
            Utils::CheckErrors("fboRenderTexture[i]->UnsetRenderSurface()");
        }

        auto quadMtl = mtlManager.GetMaterial("PBRQuad", true);
        quadMtl->GetShader().Use();
        quadMtl->GetShader().SetInteger("outputBuffer", bufferType);
        for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
            eyeRenderTexture[i]->SetAndClearRenderSurface(nullptr);
            Utils::CheckErrors("eyeRenderTexture[i]->SetAndClearRenderSurface(nullptr)");
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);

            Utils::CheckErrors("glDisable(GL_CULL_FACE)");
            glClearColor(0.0, 0.0, 0.0, 1.0);
            Utils::CheckErrors("glClearColor(0.0, 0.0, 0.0, 1.0)");
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Utils::CheckErrors("glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)");

            const char* uniformNames[] = {
            "AlbedoSpec",
            "Normal",
            "MetRouAo",
            "Position"
            };

            for(int j = 0; j < 4; j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, fboRenderTexture[i]->texIds[j]);
                quadMtl->GetShader().SetInteger(uniformNames[j], j);
            }
            Utils::CheckErrors("Texture binding");

            glm::vec3 viewPos = Utils::ConvertVec3(shiftedEyePositions[i]);
            quadMtl->GetShader().SetVector3f("viewPos", viewPos);
            Utils::CheckErrors("Viewpos binding");

            primitiveRenderer.RenderQuad();
            Utils::CheckErrors("primitiveRenderer.RenderQuad()");
            glEnable(GL_CULL_FACE);
            Utils::CheckErrors("glEnable(GL_CULL_FACE)");
            eyeRenderTexture[i]->UnsetRenderSurface();
            Utils::CheckErrors("eyeRenderTexture[i]->UnsetRenderSurface()");
            eyeRenderTexture[i]->Commit();
        }
    }

    void OVRRenderer::RenderMirror() {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        int w = context->GetWidth();
        int h = context->GetHeight();
        glBlitFramebuffer(0, h, w, 0,
                          0, 0, w, h,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    void OVRRenderer::Render(RenderCommand cmd, OVR::Matrix4f proj, OVR::Matrix4f view) {
        Utils::CheckErrors("OVRRenderer::Render");

        auto cmdMtl = mtlManager.GetMaterial(cmd.material, true);
        auto mtl = mtlManager.defaultMaterial;
        mtl->SetUniforms(cmd.uniforms);

        /*
         * uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
         */

        auto samplers = cmdMtl->GetSamplers();
//
//        int i = 0;
//        for(auto item : samplers) {
//            mtl->GetShader().SetInteger((item.first + "Map").c_str(), i);
//            glActiveTexture(GL_TEXTURE0 + i);
//            item.second->Bind();
//            i++;
//        }

        BindSampler(samplers, mtl, "Albedo", 0);
        BindSampler(samplers, mtl, "Ambient", 1);
        BindSampler(samplers, mtl, "Metallic", 2);
        BindSampler(samplers, mtl, "Normal", 3);
        BindSampler(samplers, mtl, "Roughness", 4);


        mtl->GetShader().SetMatrix4("model", cmd.model);
        mtl->GetShader().SetInteger("outputBuffer", bufferType);
        glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
        glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);
        BaseRenderer::Render(cmd);
        Utils::CheckErrors("BaseRenderer::Render(cmd)");
    }

    void OVRRenderer::BindSampler(std::map<std::string, Texture*> textures, OmicronMaterial* mtl, std::string name, int id) {
        unsigned int texId = -1;
        if(textures.find(name) != textures.end())
            texId = textures[name]->id;
        else texId = primitiveRenderer.BlankTexture();
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texId);
        mtl->GetShader().SetInteger((name + "Map").c_str(), id);
    }


}