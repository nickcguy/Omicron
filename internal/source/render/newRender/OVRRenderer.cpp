//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/OVRRenderer.hpp>
#include <render/OpenGLContext.hpp>
#include <render/IRenderProvider.hpp>
#include <render/ovr/OVRContext.hpp>
#include <utils/TextUtils.hpp>
#include <utils/MathsUtils.hpp>

#define USE_PREPASS false
namespace Omicron {

    OVRRenderer::OVRRenderer(OpenGLContext* context) : BaseRenderer(context) {}

    void OVRRenderer::Init() {

        mtlManager.SetOnNewMaterial([=](OmicronMaterial* mtl) {
            printf("New material registered: %s\n", mtl->GetName().c_str());
            mtl->InvalidateSamplers();
            mtl->GetShader().CompileFromSources();
            mtl->SetReady(true);
        });

        #if USE_PREPASS

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("PBRDefault");
        defMtl->GetShader().CompileFromFile("assets/shaders/ovr/prepass/basic.vert", "assets/shaders/ovr/prepass/basic.frag");
        defMtl->SetReady(true);

        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("PBRQuad");
        quadMtl->GetShader().CompileFromFile("assets/shaders/ovr/prepass/quad.vert", "assets/shaders/ovr/prepass/quad.frag");
        quadMtl->SetReady(true);

        OmicronMaterial* lightMtl = new OmicronMaterial;
        lightMtl->SetName("PBRLight");
        lightMtl->GetShader().CompileFromFile("assets/shaders/ovr/prepass/light.vert", "assets/shaders/ovr/prepass/light.frag");
        lightMtl->SetReady(true);

        #else

        OmicronMaterial* defMtl = new OmicronMaterial;
        defMtl->SetName("PBRDefault");
        defMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/basic.vert", "assets/shaders/ovr/deferred_pbr/basic.frag");
        defMtl->SetReady(true);

        OmicronMaterial* quadMtl = new OmicronMaterial;
        quadMtl->SetName("PBRQuad");
        quadMtl->GetShader().CompileFromFile("assets/shaders/ovr/deferred_pbr/quad.vert", "assets/shaders/ovr/deferred_pbr/quad.frag");
        quadMtl->SetReady(true);

        #endif

        mtlManager.SetDefaultMaterial(defMtl);
        mtlManager.RegisterMaterial(quadMtl);

        #if USE_PREPASS
        mtlManager.RegisterMaterial(lightMtl);
        #endif

        mtlManager.RegisterMaterials();

        Resize(context->GetWidth(), context->GetHeight());

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void OVRRenderer::Update(float delta) {
        BaseRenderer::Update(delta);
        Utils::CheckErrors("mtlManager.Update(delta)");
    }

    void OVRRenderer::Render(std::vector<RenderCommand> cmds) {
        ovr_GetSessionStatus(ovrContext->GetSession(), &sessionStatus);
        if(sessionStatus.ShouldQuit) {
            context->SetWindowShouldClose(true);
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
        InitMirror(width, height);
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
                skyRenderTexture[i] = new ovr::TextureBuffer(session, true, false, idealTexSize, 1, NULL, 1);
                #if USE_PREPASS
                fboLightRenderTexture[i] = new ovr::TextureBuffer(session, true, false, idealTexSize, 1, NULL, 1);
                #endif
            }

            Utils::CheckErrors("FBO");

            layer.Viewport[ovrEye_Left]  = OVR::Recti(0, 0,                bufferSize.w / 2, bufferSize.h);
            layer.Viewport[ovrEye_Right] = OVR::Recti(bufferSize.w / 2, 0, bufferSize.w / 2, bufferSize.h);

            readyForMirror = true;

            InitMirror(context->GetWidth(), context->GetHeight());

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
        std::vector<RenderCommand> lightCmds;

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
            Utils::CheckErrors("glDepthMask(GL_TRUE)");

            fboRenderTexture[i]->UnsetRenderSurface();
            Utils::CheckErrors("fboRenderTexture[i]->UnsetRenderSurface()");

            glDepthMask(GL_TRUE);

            skyRenderTexture[i]->SetAndClearRenderSurface(fboDepthBuffer[i], false);
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);
            RenderCubemap(proj, view);
            glEnable(GL_CULL_FACE);
            glDepthFunc(GL_LESS);
            skyRenderTexture[i]->UnsetRenderSurface();
        }
        auto lights = context->GetRenderProvider()->Lights();

        #if USE_PREPASS

        auto lightMtl = mtlManager.GetMaterial("PBRLight", true);
        lightMtl->GetShader().Use();
        lightMtl->GetShader().SetInteger("outputBuffer", bufferType);

        // TODO fix missing depth testing
        for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
            fboLightRenderTexture[i]->SetAndClearRenderSurface(fboDepthBuffer[i], false);
            glDisable(GL_CULL_FACE);
//            glDisable(GL_BLEND);
            glDepthMask(GL_FALSE);

            const char* uniformNames[] = {
            "AlbedoSpec",
            "Normal",
            "MetRouAo",
            "Position"
            };

            for(int j = 0; j < 4; j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, fboRenderTexture[i]->texIds[j]);
                lightMtl->GetShader().SetInteger(uniformNames[j], j);
            }
            Utils::CheckErrors("Texture binding");

            glm::vec3 viewPos = Utils::ConvertVec3(shiftedEyePositions[i]);
            lightMtl->GetShader().SetVector3f("viewPos", viewPos);
            Utils::CheckErrors("Viewpos binding");

            for(int l0 = 0; l0 < lights.size(); l0+=16) {
                for(int l1 = 0; l1 < 1 && l0 + l1 < lights.size(); l1++) {
                    Light l = lights[l0 + l1];
                    std::stringstream ss;
                    ss << "lights[" << l1 << "]";
                    std::string prefix = ss.str();
                    lightMtl->GetShader().SetVector3f((prefix + ".Position").c_str(), l.position);
                    lightMtl->GetShader().SetVector3f((prefix + ".Colour").c_str(), l.colour);
                    lightMtl->GetShader().SetFloat((prefix + ".Constant").c_str(), l.constant);
                    lightMtl->GetShader().SetFloat((prefix + ".Linear").c_str(), l.linear);
                    lightMtl->GetShader().SetFloat((prefix + ".Quadratic").c_str(), l.quadratic);
                    lightMtl->GetShader().SetFloat((prefix + ".Intensity").c_str(), l.intensity);
                }
                primitiveRenderer.RenderQuad();
            }

            Utils::CheckErrors("primitiveRenderer.RenderQuad()");
            glEnable(GL_CULL_FACE);
            Utils::CheckErrors("glEnable(GL_CULL_FACE)");
            fboLightRenderTexture[i]->UnsetRenderSurface();
        }
        #endif

        auto quadMtl = mtlManager.GetMaterial("PBRQuad", true);
        quadMtl->GetShader().Use();
        quadMtl->GetShader().SetInteger("outputBuffer", bufferType);
        for(int i = ovrEye_Left; i <= ovrEye_Right; i++) {
            eyeRenderTexture[i]->SetAndClearRenderSurface(nullptr);
            Utils::CheckErrors("eyeRenderTexture[i]->SetAndClearRenderSurface(nullptr)");
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);

            Utils::CheckErrors("glDisable(GL_CULL_FACE)");
            glClearColor(0.0, 0.0, 0.0, 0.0);
            Utils::CheckErrors("glClearColor(0.0, 0.0, 0.0, 1.0)");
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Utils::CheckErrors("glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)");

            glm::vec3 viewPos = Utils::ConvertVec3(shiftedEyePositions[i]);
            quadMtl->GetShader().SetVector3f("viewPos", viewPos);

            #if USE_PREPASS
            const char* uniformNames[] = {
            "AlbedoSpec",
            "Lighting"
            };

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fboRenderTexture[i]->texIds[0]);
            quadMtl->GetShader().SetInteger(uniformNames[0], 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboLightRenderTexture[i]->texIds[0]);
            quadMtl->GetShader().SetInteger(uniformNames[1], 1);

            #else
            const char* uniformNames[] = {
            "AlbedoSpec",
            "Normal",
            "MetRouAo",
            "Position"
            };

            int j = 0;
            for(j = 0; j < 4; j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, fboRenderTexture[i]->texIds[j]);
                quadMtl->GetShader().SetInteger(uniformNames[j], j);
            }

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, skyRenderTexture[i]->texIds[0]);
            quadMtl->GetShader().SetInteger("Skybox", 5);

            static glm::vec3 zero(0.f);

            for(int l0 = 0; l0 < lights.size(); l0++) {
                std::stringstream ss;
                ss << "lights[" << l0 << "]";
                std::string prefix = ss.str();
                if(l0 < lights.size()) {
                    Light light = lights[l0];
                    quadMtl->GetShader().SetInteger((prefix + ".Active").c_str(), GL_TRUE);
                    quadMtl->GetShader().SetInteger((prefix + ".Type").c_str(), light.type);

                    quadMtl->GetShader().SetVector3f((prefix + ".Position").c_str(), light.position);
                    quadMtl->GetShader().SetVector3f((prefix + ".Direction").c_str(), light.direction);
                    quadMtl->GetShader().SetVector3f((prefix + ".Colour").c_str(), light.colour);

                    quadMtl->GetShader().SetFloat((prefix + ".Constant").c_str(), light.constant);
                    quadMtl->GetShader().SetFloat((prefix + ".Linear").c_str(), light.linear);
                    quadMtl->GetShader().SetFloat((prefix + ".Quadratic").c_str(), light.quadratic);

                    quadMtl->GetShader().SetFloat((prefix + ".Cutoff").c_str(), light.cutoff);
                    quadMtl->GetShader().SetFloat((prefix + ".OuterCutoff").c_str(), light.outerCutoff);

                    quadMtl->GetShader().SetFloat((prefix + ".Intensity").c_str(), light.intensity);
                }else{
                    quadMtl->GetShader().SetInteger((prefix + ".Active").c_str(), GL_FALSE);
                }
            }

            #endif

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

    void OVRRenderer::InitMirror(size_t width, size_t height) {
        if(!readyForMirror) return;

        if(mirrorTexture) {
            ovr_DestroyMirrorTexture(ovrContext->GetSession(), mirrorTexture);
        }

        ovrMirrorTextureDesc mirrorDesc = {};
        memset(&mirrorDesc, 0, sizeof(mirrorDesc));
        mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
        mirrorDesc.Width = width;
        mirrorDesc.Height = height;
        ovr_CreateMirrorTextureGL(ovrContext->GetSession(), &mirrorDesc, &mirrorTexture);

        unsigned int texId;
        ovr_GetMirrorTextureBufferGL(ovrContext->GetSession(), mirrorTexture, &texId);
        if(mirrorFBO == 0) glGenFramebuffers(1, &mirrorFBO);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
        glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        Utils::CheckErrors("Mirror");
    }


}