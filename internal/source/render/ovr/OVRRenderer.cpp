//
// Created by Guy on 07/08/2017.
//

#include <render/ovr/OVRRenderer.hpp>
#include <render/OpenGLContext.hpp>
#include <utils/MathsUtils.hpp>
#include <sstream>
#include <engine/input/OVRInputProvider.hpp>
#include <engine/component/PhysicsComponent.hpp>
#include <GLFW/glfw3.h>

namespace Omicron {

    namespace ovr {

        #define LEFT_EYE ovrEye_Left
        #define RIGHT_EYE ovrEye_Right


        OVRRenderer::OVRRenderer(Omicron::OpenGLContext* context) : Omicron::OpenGLRenderer(context), cmdProcessor(this) {
            DEBUG_PRINT(context->inputProvider = new OVRInputProvider);
        }

        void OVRRenderer::Init() {
            DEBUG_PRINT(OpenGLRenderer::Init());

            ovrResult result = ovr_Initialize(nullptr);
            if(OVR_FAILURE(result))
                throw std::runtime_error("OVR failed to initialize");

            result = ovr_Create(&session, &luid);
            if(OVR_FAILURE(result)) {
                ovr_Shutdown();
                throw std::runtime_error("OVR failed to create session");
            }

            desc = ovr_GetHmdDesc(session);
            printf("OVR HMD Description: \n");
            printf("\tType: %i\n", desc.Type);
            printf("\tRefresh rate: %f\n", desc.DisplayRefreshRate);
            printf("\tFirmware major: %i\n", desc.FirmwareMajor);
            printf("\tFirmware minor: %i\n", desc.FirmwareMinor);
            printf("\tManufacturer: %s\n", desc.Manufacturer);
            printf("\tProduct ID: %i\n", desc.ProductId);
            printf("\tProduct Name: %s\n", desc.ProductName);
            printf("\tSerial number: %s\n", desc.SerialNumber);
            printf("\tVendor ID: %i\n", desc.VendorId);
            printf("\tResolution: [%i, %i]\n", desc.Resolution.w, desc.Resolution.h);

            float pixelDensity = 1.0f;

            OVR::Sizei tex0Size = ovr_GetFovTextureSize(session, ovrEye_Left, desc.DefaultEyeFov[0], pixelDensity);
            OVR::Sizei tex1Size = ovr_GetFovTextureSize(session, ovrEye_Right, desc.DefaultEyeFov[1], pixelDensity);

            OVR::Sizei bufferSize;
            bufferSize.w = tex0Size.w + tex1Size.w;
            bufferSize.h = max(tex0Size.h, tex1Size.h);

            ovrTextureSwapChainDesc swapChainDesc = {};
            swapChainDesc.Type = ovrTexture_2D;
            swapChainDesc.ArraySize = 1;
            swapChainDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
            swapChainDesc.Width = bufferSize.w;
            swapChainDesc.Height = bufferSize.h;
            swapChainDesc.MipLevels = 1;
            swapChainDesc.SampleCount = 1;
            swapChainDesc.StaticImage = ovrFalse;

            if(ovr_CreateTextureSwapChainGL(session, &swapChainDesc, &swapChain) == ovrSuccess) {
                unsigned int texId;
                ovr_GetTextureSwapChainBufferGL(session, swapChain, 0, &texId);
                glBindTexture(GL_TEXTURE_2D, texId);
            }

            eyeRenderDesc[LEFT_EYE]  = ovr_GetRenderDesc(session, LEFT_EYE,  desc.DefaultEyeFov[LEFT_EYE]);
            eyeRenderDesc[RIGHT_EYE] = ovr_GetRenderDesc(session, RIGHT_EYE, desc.DefaultEyeFov[RIGHT_EYE]);
            hmdToEyeViewOffset[LEFT_EYE]  = eyeRenderDesc[LEFT_EYE].HmdToEyeOffset;
            hmdToEyeViewOffset[RIGHT_EYE] = eyeRenderDesc[RIGHT_EYE].HmdToEyeOffset;

            layer.Header.Type = ovrLayerType_EyeFov;
            layer.Header.Flags = 0;
            layer.ColorTexture[LEFT_EYE] = swapChain;
            layer.ColorTexture[RIGHT_EYE] = swapChain;
            layer.Fov[LEFT_EYE] = eyeRenderDesc[LEFT_EYE].Fov;
            layer.Fov[RIGHT_EYE] = eyeRenderDesc[RIGHT_EYE].Fov;
            layer.Viewport[LEFT_EYE]  = OVR::Recti(0, 0,                bufferSize.w / 2, bufferSize.h);
            layer.Viewport[RIGHT_EYE] = OVR::Recti(bufferSize.w / 2, 0, bufferSize.w / 2, bufferSize.h);

            projections[LEFT_EYE] = ovrMatrix4f_Projection(desc.DefaultEyeFov[LEFT_EYE], 0.2f, 1000.f, ovrProjection_None);
            projections[RIGHT_EYE] = ovrMatrix4f_Projection(desc.DefaultEyeFov[RIGHT_EYE], 0.2f, 1000.f, ovrProjection_None);

            for(int eye = 0; eye < 2; eye++) {
                OVR::Sizei idealTexSize = ovr_GetFovTextureSize(session, eyeRenderDesc[eye].Eye, desc.DefaultEyeFov[eye], 1);
                eyeRenderTexture[eye] = new TextureBuffer(session, true, true, idealTexSize, 1, NULL, 1);
                eyeDepthBuffer[eye] = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);

                fboRenderTexture[eye] = new TextureBuffer(session, true, false, idealTexSize, 1, NULL, 1, 5);
                fboDepthBuffer[eye] = new DepthBuffer(fboRenderTexture[eye]->GetSize(), 0);
            }

            ovrMirrorTextureDesc mirrorDesc;
            memset(&mirrorDesc, 0, sizeof(mirrorDesc));
            mirrorDesc.Width = context->GetWidth();
            mirrorDesc.Height = context->GetHeight();
            mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

            ovr_CreateMirrorTextureGL(session, &mirrorDesc, &mirrorTexture);

            unsigned int texId;
            ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId);
            glGenFramebuffers(1, &mirrorFBO);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
            glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

            ovr_SetTrackingOriginType(session, ovrTrackingOrigin_FloorLevel);

            cmdProcessor.Init(800, 600);
            cmdProcessor.ResizeLeft(tex0Size.w, tex0Size.h);
            cmdProcessor.ResizeRight(tex1Size.w, tex1Size.h);

        }

        void OVRRenderer::PreRender(float delta) {
            UpdateTracking();

            double displayMidpointSeconds = ovr_GetPredictedDisplayTime(session, 0);
            ovrTrackingState hmdState = ovr_GetTrackingState(session, displayMidpointSeconds, ovrTrue);
            ovr_CalcEyePoses(hmdState.HeadPose.ThePose, hmdToEyeViewOffset, layer.RenderPose);

//            OpenGLRenderer::PreRender(delta);

            SSBOLightData data = lightData.Flatten();
            lightBuffer.SetData(sizeof(data), (void*)&data);

            size_t mat4Size = sizeof(glm::mat4);

//        matrixBuffer.SetUniformMatrix(matrices.projection, mat4Size, 0);
//        matrixBuffer.SetUniformMatrix(matrices.view, mat4Size, mat4Size);
            mtlManager.Update(delta);

        }

        void OVRRenderer::Render(RenderCommand cmd) {}

        void OVRRenderer::Shutdown() {
            OpenGLRenderer::Shutdown();
            ovr_Destroy(session);
            ovr_Shutdown();
        }

        void OVRRenderer::UpdateTracking() {
            trackingState = ovr_GetTrackingState(session, ovr_GetTimeInSeconds(), ovrTrue);

            if(trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {

                headPose = trackingState.HeadPose.ThePose;

                auto headPos = headPose.Translation;
                headPos -= trackingState.CalibratedOrigin.Position;

                context->GetCamera()->Position.x = headPos.x;
                context->GetCamera()->Position.y = headPos.y;
                context->GetCamera()->Position.z = headPos.z;

                auto poseQuat = headPose.Rotation;

                float roll;
                float pitch;
                float yaw;
                poseQuat.GetEulerAngles<OVR::Axis::Axis_X, OVR::Axis::Axis_Y, OVR::Axis::Axis_Z>(&yaw, &pitch, &roll);

                context->GetCamera()->Roll = roll;
                context->GetCamera()->Pitch = pitch;
                context->GetCamera()->Yaw = -yaw;

//                context->GetCamera()->SetRotation(Utils::ConvertQuat(poseQuat.Conj()));
            }


            for(int hand = 0; hand < 2; hand++) {
                ovrInputState state;
                ovr_GetInputState(session, ovrControllerType_Touch, &state);

                if(context->inputProvider->GetType() == OVR) {
                    OVRInputProvider* inputProvider = static_cast<OVRInputProvider*>(context->inputProvider);
                    inputProvider->trackingState = trackingState;
                    inputProvider->inputData = state;
//                    inputProvider->translationOffset = context->GetCamera().Position;
                }

                OmicronMaterial* mtl = mtlManager.GetMaterial("Oak Floor", false);

                if(trackingState.HandStatusFlags[hand] & ovrStatus_PositionTracked) {
                    if(handEntities[hand]) {
                        OVR::Posef handPose = trackingState.HandPoses[hand].ThePose;

//                        handEntities[hand]->transform.Translation = Utils::ConvertVec3(handPose.Translation) + context->GetCamera().Position;
                        handEntities[hand]->transform.useMatrix = true;
                        handEntities[hand]->transform.Scale = {0.1f, 0.1f, 0.1f};

                        if(handEntities[hand]->HasComponent<PhysicsComponent>()) {
                            PhysicsComponent* phys = handEntities[hand]->GetCastComponent<PhysicsComponent>();
                            btTransform trans = phys->GetBody()->getWorldTransform();
                            glm::vec3 pos = handEntities[hand]->transform.Translation;
                            glm::quat rot = handEntities[hand]->transform.Rotation;
                            trans.setOrigin({pos.x, pos.y, pos.z});
                            trans.setRotation({rot.w, rot.x, rot.y, rot.z});
                            phys->GetBody()->setWorldTransform(trans);
                            phys->GetBody()->setGravity({0.f, -2.f, 0.f});
                            phys->GetBody()->clearForces();
                        }
                    }
                }
                if(trackingState.HandStatusFlags[hand] & ovrStatus_OrientationTracked) {
                    if(handEntities[hand]) {
                        OVR::Posef handPose = trackingState.HandPoses[hand].ThePose;
                        handEntities[hand]->transform.Rotation = Utils::ConvertQuat(handPose.Rotation);
                        handEntities[hand]->transform.useMatrix = true;
                        handEntities[hand]->transform.Scale = {0.1f, 0.1f, 0.1f};
                    }

                }
            }
        }

        bool OVRRenderer::HandlesOwnLoop() {
            return true;
        }

        void OVRRenderer::Render(std::vector<RenderCommand> cmds) {


            ovrSessionStatus sessionStatus;
            ovr_GetSessionStatus(session, &sessionStatus);
            if(sessionStatus.ShouldQuit) {
                glfwSetWindowShouldClose(context->GetWindow(), true);
                return;
            }
            if(sessionStatus.ShouldRecenter)
                ovr_RecenterTrackingOrigin(session);

            if(sessionStatus.IsVisible) {

                ovrEyeRenderDesc eyeRenderDesc[2];
                eyeRenderDesc[LEFT_EYE]  = ovr_GetRenderDesc(session, LEFT_EYE,  desc.DefaultEyeFov[LEFT_EYE]);
                eyeRenderDesc[RIGHT_EYE] = ovr_GetRenderDesc(session, RIGHT_EYE, desc.DefaultEyeFov[RIGHT_EYE]);

                ovrPosef EyeRenderPose[2];
                ovrVector3f HmdToEyeOffset[2];
                HmdToEyeOffset[LEFT_EYE] = eyeRenderDesc[LEFT_EYE].HmdToEyeOffset;
                HmdToEyeOffset[RIGHT_EYE] = eyeRenderDesc[RIGHT_EYE].HmdToEyeOffset;

                double sensorSampleTime;
                ovr_GetEyePoses(session, frameIndex, ovrTrue, HmdToEyeOffset, EyeRenderPose, &sensorSampleTime);

                if(useFBO) RenderFBO(cmds, EyeRenderPose);
                else RenderDirect(cmds, EyeRenderPose);


                ovrLayerEyeFov ld;
                ld.Header.Type = ovrLayerType_EyeFov;
                ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

                for(int eye = 0; eye < 2; eye++) {
                    ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureChain;
                    ld.Viewport[eye] = OVR::Recti(eyeRenderTexture[eye]->GetSize());
                    ld.Fov[eye] = desc.DefaultEyeFov[eye];
                    ld.RenderPose[eye] = EyeRenderPose[eye];
                    ld.SensorSampleTime = sensorSampleTime;
                }

                const ovrLayerHeader* layers = &ld.Header;
                ovr_SubmitFrame(session, frameIndex, nullptr, &layers, 1);

                frameIndex++;
            }

            glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            int w = windowWidth;
            int h = windowHeight;
            glBlitFramebuffer(0, h, w, 0,
                              0, 0, w, h,
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            unsigned int id = cmdProcessor.RenderLeft(cmds, Utils::ConvertMat4(matrices.projection), Utils::ConvertMat4(matrices.view));
//
//            OmicronMaterial* quadMtl = mtlManager.GetMaterial("Textured Screen Quad");
//
//            quadMtl->GetShader().Use();
//
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, id);
//            quadMtl->GetShader().SetInteger("image", 0);
//            primitiveRenderer.RenderQuad();
        }

        void OVRRenderer::SetHandEntity(ovrHandType hand, OmicronEntity* entity) {
            handEntities[hand] = entity;
        }

        OVR::Posef OVRRenderer::VirtualWorldTransformfromRealPose(const Posef& sensorHeadPose, ovrTrackingOrigin trackingOrigin) {
            Quatf baseQ = Quatf(Vector3f(0,1,0), context->GetCamera()->Yaw);

            Vector3f BodyPos(7.7f, 1.76f - 0.15f, -1.0f);
            Vector3f BodyPoseFloorLevel(7.7f, 0.0f, -1.0f);

            Vector3f bodyPosInOrigin = trackingOrigin == ovrTrackingOrigin_EyeLevel ? BodyPos : BodyPoseFloorLevel;
            return Posef(baseQ * sensorHeadPose.Rotation, bodyPosInOrigin + baseQ.Rotate(sensorHeadPose.Translation));
        }

        void OVRRenderer::RenderFBO(std::vector<RenderCommand> cmds, ovrPosef EyeRenderPose[2]) {

            Vector3f Pos = {
            context->GetCamera()->Position.x,
            context->GetCamera()->Position.y,
            context->GetCamera()->Position.z,
            };

            static float Yaw(0.f);

            std::vector<RenderCommand> solidCmds;
            std::vector<RenderCommand> alphaCmds;

            for(RenderCommand cmd : cmds) {
                if(cmd.alpha == 1.0)
                    solidCmds.push_back(cmd);
                else alphaCmds.push_back(cmd);
            }

            for(int eye = 0; eye < 2; eye++) {

                OVR::Matrix4f rollPitchYaw = OVR::Matrix4f::RotationY(Yaw);
                OVR::Matrix4f finalRollPitchYaw = rollPitchYaw * OVR::Matrix4f(EyeRenderPose[eye].Orientation);
                OVR::Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
                OVR::Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
                OVR::Vector3f shiftedEyePos = Pos + rollPitchYaw.Transform(EyeRenderPose[eye].Position);

                auto fovPort = desc.DefaultEyeFov[eye];
                float vertFov = fovPort.UpTan + fovPort.DownTan;

                OVR::Matrix4f proj = ovrMatrix4f_Projection(desc.DefaultEyeFov[eye], 0.1f, 1000.f, ovrProjection_None);
                OVR::Matrix4f view = OVR::Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);

                fboRenderTexture[eye]->SetAndClearRenderSurface(fboDepthBuffer[eye]);
                glClearColor(eye, 1-eye, 0.f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                for(RenderCommand cmd : solidCmds)
                    Render(cmd, proj, view);
                glDepthMask(GL_FALSE);
                glBlendFunc(GL_ONE, GL_ONE);
                for(RenderCommand cmd : alphaCmds)
                    Render(cmd, proj, view);
                glDepthMask(GL_TRUE);
                fboRenderTexture[eye]->UnsetRenderSurface();

            }
            OmicronMaterial* quadMtl = mtlManager.GetMaterial("Textured Screen Quad", true);
            quadMtl->GetShader().Use();

            for(int eye = 0; eye < 2; eye++) {

                eyeRenderTexture[eye]->SetAndClearRenderSurface(nullptr);
                glDisable(GL_CULL_FACE);
                glClearColor(eye, 1-eye, 0.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                const char* uniformNames[] = {
                    "AlbedoSpec",
                    "Normal",
                    "MetRouAo",
                    "Position",
                    "W_TexCoords"
                };

                for(int i = 0; i < 5; i++) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, fboRenderTexture[eye]->texIds[i]);
                    quadMtl->GetShader().SetInteger(uniformNames[i], i);
                }
                quadMtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));
                primitiveRenderer.RenderQuad();

                eyeRenderTexture[eye]->UnsetRenderSurface();
                eyeRenderTexture[eye]->Commit();
            }
        }

        void OVRRenderer::RenderDirect(std::vector<RenderCommand> cmds, ovrPosef EyeRenderPose[2]) {

            Vector3f Pos = {
            context->GetCamera()->Position.x,
            context->GetCamera()->Position.y,
            context->GetCamera()->Position.z,
            };

            static float Yaw(0.f);

            for(int eye = 0; eye < 2; eye++) {

                eyeRenderTexture[eye]->SetAndClearRenderSurface(eyeDepthBuffer[eye]);

                OVR::Matrix4f rollPitchYaw = OVR::Matrix4f::RotationY(Yaw);
                OVR::Matrix4f finalRollPitchYaw = rollPitchYaw * OVR::Matrix4f(EyeRenderPose[eye].Orientation);
                OVR::Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
                OVR::Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
                OVR::Vector3f shiftedEyePos = Pos + rollPitchYaw.Transform(EyeRenderPose[eye].Position);

                auto fovPort = desc.DefaultEyeFov[eye];
                float vertFov = fovPort.UpTan + fovPort.DownTan;

                OVR::Matrix4f proj = ovrMatrix4f_Projection(desc.DefaultEyeFov[eye], 0.1f, 1000.f, ovrProjection_None);
                OVR::Matrix4f view = OVR::Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);

                for(RenderCommand cmd : cmds)
                    Render(cmd, proj, view);

                eyeRenderTexture[eye]->UnsetRenderSurface();

                eyeRenderTexture[eye]->Commit();

            }
        }

        void OVRRenderer::Render(RenderCommand cmd, OVR::Matrix4f& proj, OVR::Matrix4f& view, bool toDeferred) {
            auto mtl = mtlManager.GetMaterial(cmd.material, cmd.materialInstance, true);


            if(toDeferred != (glfwGetKey(context->GetWindow(), GLFW_KEY_G) == GLFW_PRESS)) {
                auto deferredMtl = mtlManager.GetMaterialBase("Deferred", true);
                auto samplers = mtl->GetSamplers();
                deferredMtl->SetUniforms();
//                PostIncludes(deferredMtl);

                int index = 0;
                for(auto pair : samplers) {
                    glActiveTexture(GL_TEXTURE0 + index);
                    pair.second->Bind();
                    deferredMtl->GetShader().SetInteger((pair.first + "Map").c_str(), index);
                    index++;
                }

                glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
                glUniformMatrix4fv(glGetUniformLocation(deferredMtl->GetShader().ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);

                deferredMtl->GetShader().SetMatrix4("model", cmd.model);
                deferredMtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));
            }else{
                mtl->SetUniforms();
                PostIncludes(mtl);

                glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "view"), 1, GL_TRUE, (FLOAT*) &view);
                glUniformMatrix4fv(glGetUniformLocation(mtl->GetShader().ID, "projection"), 1, GL_TRUE, (FLOAT*) &proj);

                mtl->GetShader().SetMatrix4("model", cmd.model);
                mtl->GetShader().SetInteger("outputBuffer", static_cast<int>(bufferType));
            }

            glBindVertexArray(cmd.VAO);
            switch(cmd.type) {
                case VERTEX:
                case POINTCLOUD:
                    glDrawArrays(cmd.primitive, cmd.offset, cmd.size);
                    break;
                case INDEXED:
                    glDrawElements(cmd.primitive, cmd.size, cmd.indexType, (void*) cmd.offset);
                    break;
                default:
                    break;
            }
        }

    }
}