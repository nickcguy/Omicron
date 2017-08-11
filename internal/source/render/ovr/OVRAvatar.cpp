//
// Created by Guy on 07/08/2017.
//

#include <render/ovr/OVRAvatar.hpp>
#include <utils/MathsUtils.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace Omicron {

    namespace ovr {

        Avatar::Avatar() {
            stdShader.CompileFromFile("assets/ovr/shaders/AvatarVertexShader.glsl", "assets/ovr/shaders/AvatarFragmentShader.glsl");
            pbsShader.CompileFromFile("assets/ovr/shaders/AvatarVertexShader.glsl", "assets/ovr/shaders/AvatarFragmentShaderPBS.glsl");
        }

        void Avatar::BuildAvatar() {
            AvatarHost::Instance().localUserId = ovr_GetLoggedInUserID();
            BuildAvatar(AvatarHost::Instance().localUserId);
        }

        void Avatar::BuildAvatar(ovrID userId) {
            this->userId = userId;
            AvatarHost::Instance().RegisterAvatar(userId, this);
            ovrAvatar_RequestAvatarSpecification(userId);
        }

        void Avatar::SetAvatar(ovrAvatar* avatar) {
            this->avatar = avatar;
        }

        void Avatar::Render(uint32_t visibilityMask, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool renderJoints) {
            if(avatar) {
                uint32_t componentCount = ovrAvatarComponent_Count(avatar);
                for(uint32_t i = 0; i < componentCount; i++) {
                    const ovrAvatarComponent* component = ovrAvatarComponent_Get(avatar, i);

                    glm::mat4 world = Utils::ConvertOVRAvatarTransform(component->transform);
                    for(uint32_t j = 0; j < component->renderPartCount; j++) {
                        const ovrAvatarRenderPart* renderPart = component->renderParts[j];
                        ovrAvatarRenderPartType type = ovrAvatarRenderPart_GetType(renderPart);
                        switch(type) {
                            case ovrAvatarRenderPartType_SkinnedMeshRender:
                                RenderSkinnedMeshPart(ovrAvatarRenderPart_GetSkinnedMeshRender(renderPart), visibilityMask, world, view, proj, viewPos, renderJoints);
                                break;
                            case ovrAvatarRenderPartType_SkinnedMeshRenderPBS:
                                RenderSkinnedMeshPartPBS(ovrAvatarRenderPart_GetSkinnedMeshRenderPBS(renderPart), visibilityMask, world, view, proj, viewPos, renderJoints);
                                break;
                            case ovrAvatarRenderPartType_ProjectorRender:
                                RenderProjector(ovrAvatarRenderPart_GetProjectorRender(renderPart), avatar, visibilityMask, world, view, proj, viewPos);
                                break;
                        }
                    }
                }
            }
        }

        void Avatar::RenderSkinnedMeshPart(const ovrAvatarRenderPart_SkinnedMeshRender* mesh, uint32_t visibilityMask,
                                           const glm::mat4& world, const glm::mat4& view, const glm::mat4 proj,
                                           const glm::vec3& viewPos, bool renderJoints) {

            if(mesh->visibilityMask & visibilityMask == 0)
                return;

            MeshData* data = (MeshData*)AvatarHost::Instance().GetAsset(mesh->meshAssetID);
            stdShader.Use();
            SetMeshState(stdShader.ID, mesh->localTransform, data, mesh->skinnedPose, world, view, proj, viewPos);
            SetMaterialState(stdShader.ID, &mesh->materialState, nullptr);

            glBindVertexArray(data->vertexArray);
            glDepthFunc(GL_LESS);

            if(mesh->visibilityMask & ovrAvatarVisibilityFlag_SelfOccluding) {
                glDepthMask(GL_TRUE);
                glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                glDrawElements(GL_TRIANGLES, data->elementCount, GL_UNSIGNED_SHORT, 0);
                glDepthFunc(GL_EQUAL);
            }

            glDepthMask(GL_FALSE);
            glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDrawElements(GL_TRIANGLES, data->elementCount, GL_UNSIGNED_SHORT, 0);
            glBindVertexArray(0);

            if(renderJoints) {
                glm::mat4 local = Utils::ConvertOVRAvatarTransform(mesh->localTransform);
                glDepthFunc(GL_ALWAYS);
                RenderPose(proj * view * world * local, mesh->skinnedPose);
            }

        }

        void Avatar::SetMeshState(GLuint program, const ovrAvatarTransform& localTransform, const MeshData* data,
                                  const ovrAvatarSkinnedMeshPose& skinnedPose, const glm::mat4& world,
                                  const glm::mat4& view, const glm::mat4 proj, const glm::vec3& viewPos) {

            glm::mat4 local = Utils::ConvertOVRAvatarTransform(localTransform);
            glm::mat4 worldMat = world * local;
            glm::mat4 viewProjMat = proj * view;

            glm::mat4* skinnedPoses = (glm::mat4*)alloca(sizeof(glm::mat4) * skinnedPose.jointCount);
            ComputeWorldPose(skinnedPose, skinnedPoses);
            for(uint32_t i = 0; i < skinnedPose.jointCount; i++)
                skinnedPoses[i] = skinnedPoses[i] * data->inverseBindPose[i];

            glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, glm::value_ptr(viewPos));

            glUniformMatrix4fv(glGetUniformLocation(program, "world"), 1, 0, glm::value_ptr(worldMat));
            glUniformMatrix4fv(glGetUniformLocation(program, "viewProj"), 1, 0, glm::value_ptr(viewProjMat));
            glUniformMatrix4fv(glGetUniformLocation(program, "meshPose"), skinnedPose.jointCount, 0, glm::value_ptr(*skinnedPoses));

        }

        void Avatar::ComputeWorldPose(const ovrAvatarSkinnedMeshPose& localPose, glm::mat4* worldPose) {
            for(uint32_t i = 0; i < localPose.jointCount; i++) {
                glm::mat4 local = Utils::ConvertOVRAvatarTransform(localPose.jointTransform[i]);
                int parentIndex = localPose.jointParents[i];
                if(parentIndex < 0) worldPose[i] = local;
                else worldPose[i] = worldPose[parentIndex] * local;
            }
        }

        void Avatar::SetMaterialState(GLuint program, const ovrAvatarMaterialState* state, glm::mat4* projectorInv) {
// Assign the fragment uniforms
            glUniform1i(glGetUniformLocation(program, "useAlpha"), state->alphaMaskTextureID != 0);
            glUniform1i(glGetUniformLocation(program, "useNormalMap"), state->normalMapTextureID != 0);
            glUniform1i(glGetUniformLocation(program, "useRoughnessMap"), state->roughnessMapTextureID != 0);

            // TODO verify this behaves correctly
            glUniform1f(glGetUniformLocation(program, "elapsedSeconds"), glfwGetTime());

            if (projectorInv) {
                glUniform1i(glGetUniformLocation(program, "useProjector"), 1);
                glUniformMatrix4fv(glGetUniformLocation(program, "projectorInv"), 1, 0, glm::value_ptr(*projectorInv));
            }else{
                glUniform1i(glGetUniformLocation(program, "useProjector"), 0);
            }

            int textureSlot = 1;
            glUniform4fv(glGetUniformLocation(program, "baseColor"), 1, &state->baseColor.x);
            glUniform1i(glGetUniformLocation(program, "baseMaskType"), state->baseMaskType);
            glUniform4fv(glGetUniformLocation(program, "baseMaskParameters"), 1, &state->baseMaskParameters.x);
            glUniform4fv(glGetUniformLocation(program, "baseMaskAxis"), 1, &state->baseMaskAxis.x);
            SetTextureSampler(program, textureSlot++, "alphaMask", state->alphaMaskTextureID);
            glUniform4fv(glGetUniformLocation(program, "alphaMaskScaleOffset"), 1, &state->alphaMaskScaleOffset.x);
            SetTextureSampler(program, textureSlot++, "normalMap", state->normalMapTextureID);
            glUniform4fv(glGetUniformLocation(program, "normalMapScaleOffset"), 1, &state->normalMapScaleOffset.x);
            SetTextureSampler(program, textureSlot++, "parallaxMap", state->parallaxMapTextureID);
            glUniform4fv(glGetUniformLocation(program, "parallaxMapScaleOffset"), 1, &state->parallaxMapScaleOffset.x);
            SetTextureSampler(program, textureSlot++, "roughnessMap", state->roughnessMapTextureID);
            glUniform4fv(glGetUniformLocation(program, "roughnessMapScaleOffset"), 1, &state->roughnessMapScaleOffset.x);

            struct LayerUniforms {
                int layerSamplerModes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                int layerBlendModes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                int layerMaskTypes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarVector4f layerColors[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                int layerSurfaces[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarAssetID layerSurfaceIDs[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarVector4f layerSurfaceScaleOffsets[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarVector4f layerSampleParameters[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarVector4f layerMaskParameters[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
                ovrAvatarVector4f layerMaskAxes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
            } layerUniforms;
            memset(&layerUniforms, 0, sizeof(layerUniforms));
            for (uint32_t i = 0; i < state->layerCount; ++i)
            {
                const ovrAvatarMaterialLayerState& layerState = state->layers[i];
                layerUniforms.layerSamplerModes[i] = layerState.sampleMode;
                layerUniforms.layerBlendModes[i] = layerState.blendMode;
                layerUniforms.layerMaskTypes[i] = layerState.maskType;
                layerUniforms.layerColors[i] = layerState.layerColor;
                layerUniforms.layerSurfaces[i] = textureSlot++;
                layerUniforms.layerSurfaceIDs[i] = layerState.sampleTexture;
                layerUniforms.layerSurfaceScaleOffsets[i] = layerState.sampleScaleOffset;
                layerUniforms.layerSampleParameters[i] = layerState.sampleParameters;
                layerUniforms.layerMaskParameters[i] = layerState.maskParameters;
                layerUniforms.layerMaskAxes[i] = layerState.maskAxis;
            }

            glUniform1i(glGetUniformLocation(program, "layerCount"), state->layerCount);
            glUniform1iv(glGetUniformLocation(program, "layerSamplerModes"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, layerUniforms.layerSamplerModes);
            glUniform1iv(glGetUniformLocation(program, "layerBlendModes"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, layerUniforms.layerBlendModes);
            glUniform1iv(glGetUniformLocation(program, "layerMaskTypes"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, layerUniforms.layerMaskTypes);
            glUniform4fv(glGetUniformLocation(program, "layerColors"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, (float*)layerUniforms.layerColors);
            SetTextureSamplers(program, "layerSurfaces", OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, layerUniforms.layerSurfaces, layerUniforms.layerSurfaceIDs);
            glUniform4fv(glGetUniformLocation(program, "layerSurfaceScaleOffsets"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, (float*)layerUniforms.layerSurfaceScaleOffsets);
            glUniform4fv(glGetUniformLocation(program, "layerSampleParameters"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, (float*)layerUniforms.layerSampleParameters);
            glUniform4fv(glGetUniformLocation(program, "layerMaskParameters"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, (float*)layerUniforms.layerMaskParameters);
            glUniform4fv(glGetUniformLocation(program, "layerMaskAxes"), OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT, (float*)layerUniforms.layerMaskAxes);
        }

        void Avatar::SetTextureSampler(GLuint program, int textureUnit, const char* uniformName, ovrAvatarAssetID assetId) {
            GLuint texId = 0;
            if(assetId) {
                void* data = AvatarHost::Instance().GetAsset(assetId);
                TextureData* textureData = static_cast<TextureData*>(data);
                texId = textureData->textureID;
            }

            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, texId);
            glUniform1i(glGetUniformLocation(program, uniformName), textureUnit);
        }

        void Avatar::SetTextureSamplers(GLuint program, const char* uniformName, size_t count, const int* textureUnits,
                                        const ovrAvatarAssetID* assetIds) {
            for(int i = 0; i < count; i++) {
                ovrAvatarAssetID assetId = assetIds[i];
                GLuint texId = 0;
                if(assetId) {
                    void* data = AvatarHost::Instance().GetAsset(assetId);
                    if(data) {
                        TextureData* textureData = static_cast<TextureData*>(data);
                        texId = textureData->textureID;
                    }
                }
                glActiveTexture(GL_TEXTURE0 + textureUnits[i]);
                glBindTexture(GL_TEXTURE_2D, texId);
            }
            GLint uniformLocation = glGetUniformLocation(program, uniformName);
            glUniform1iv(uniformLocation, count, textureUnits);
        }

        void Avatar::RenderPose(const glm::mat4& transform, const ovrAvatarSkinnedMeshPose& pose) {
            glm::mat4* skinnedPoses = (glm::mat4*)alloca(sizeof(glm::mat4) * pose.jointCount);
            ComputeWorldPose(pose, skinnedPoses);
            for(uint32_t i = 1; i < pose.jointCount; i++) {
                int parent = pose.jointParents[i];
                RenderDebugLine(transform, glm::vec3(skinnedPoses[parent][3]), glm::vec3(skinnedPoses[i][3]), glm::vec4(1), glm::vec4(1, 0, 0, 1));
            }
        }

        void Avatar::RenderDebugLine(const glm::mat4& worldViewProj, const glm::vec3& a, const glm::vec3& b,
                                     const glm::vec4& aColor, const glm::vec4& bColor) {

        }

        void AvatarHost::HandleAvatarSpecification(const ovrAvatarMessage_AvatarSpecification* msg) {

            auto avatar = ovrAvatar_Create(msg->avatarSpec, ovrAvatarCapability_All);
            userAvatarMap[msg->oculusUserID]->SetAvatar(avatar);

            uint32_t refCount = ovrAvatar_GetReferencedAssetCount(avatar);
            for(uint32_t i = 0; i < refCount; i++) {
                ovrAvatarAssetID id = ovrAvatar_GetReferencedAsset(avatar, i);
                ovrAvatarAsset_BeginLoading(id);
                loadingAssets++;
            }
            printf("Loading %d assets...\r\n", loadingAssets);
        }

        void AvatarHost::HandleAssetLoaded(const ovrAvatarMessage_AssetLoaded* msg) {
            ovrAvatarAssetType assetType = ovrAvatarAsset_GetType(msg->asset);
            void* data = nullptr;

            switch(assetType) {
                case ovrAvatarAssetType_Mesh:
                    data = LoadMesh(ovrAvatarAsset_GetMeshData(msg->asset));
                    break;
                case ovrAvatarAssetType_Texture:
                    data = LoadTexture(ovrAvatarAsset_GetTextureData(msg->asset));
                    break;
            }

            assetMap[msg->assetID] = data;
            loadingAssets--;
            printf("Loading %d assets...\r\n", loadingAssets);
        }

        MeshData* AvatarHost::LoadMesh(const ovrAvatarMeshAssetData* data) {

        }

        TextureData* AvatarHost::LoadTexture(const ovrAvatarTextureAssetData* data) {

        }

        void AvatarHost::RegisterAvatar(ovrID userId, Avatar* avatar) {
            userAvatarMap[userId] = avatar;
        }

        void AvatarHost::Update() {
            if (ovrAvatarMessage* message = ovrAvatarMessage_Pop()) {
                switch (ovrAvatarMessage_GetType(message)) {
                    case ovrAvatarMessageType_AvatarSpecification:
                        HandleAvatarSpecification(ovrAvatarMessage_GetAvatarSpecification(message));
                        break;
                    case ovrAvatarMessageType_AssetLoaded:
                        HandleAssetLoaded(ovrAvatarMessage_GetAssetLoaded(message));
                        break;
                }
                ovrAvatarMessage_Free(message);
            }
        }

        void AvatarHost::RenderAvatars(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool renderJoints) {
            for(auto item : userAvatarMap) {
                Avatar* avatar = item.second;
                ovrAvatarVisibilityFlags visibilityMask = ovrAvatarVisibilityFlag_ThirdPerson;
                if(item.first == localUserId)
                    visibilityMask = ovrAvatarVisibilityFlag_FirstPerson;
                avatar->Render(visibilityMask, view, proj, viewPos, renderJoints);
            }
        }

        void* AvatarHost::GetAsset(ovrAvatarAssetID id) {
            return assetMap[id];
        }
    }

}