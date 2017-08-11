//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_OVRAVATAR_HPP
#define OMICRONRENDER_OVRAVATAR_HPP

#include <render/shader/Shader.hpp>
#include <OVR_Avatar.h>
#include <OVR_Platform.h>
#include <pch.hpp>
#include <map>

namespace Omicron {

    namespace ovr {

        void InitializeAvatarSDK() {
            ovr_PlatformInitializeWindows(OCULUS_APP_ID);
            ovrAvatar_Initialize(OCULUS_APP_ID);
        }

        struct MeshData {
            GLuint vertexArray;
            GLuint vertexBuffer;
            GLuint elementBuffer;
            GLuint elementCount;
            glm::mat4 bindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
            glm::mat4 inverseBindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
        };

        struct TextureData {
            GLuint textureID;
        };

        class AvatarHost;

        class Avatar {
        public:

            Avatar();

            void BuildAvatar();
            void BuildAvatar(ovrID userId);

            void SetAvatar(ovrAvatar* a);
            void Render(uint32_t visibilityMask, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool renderJoints);
            void RenderPose(const glm::mat4& transform, const ovrAvatarSkinnedMeshPose& pose);
            void RenderDebugLine(const glm::mat4& worldViewProj, const glm::vec3& a, const glm::vec3& b, const glm::vec4& aColor, const glm::vec4& bColor);
            void RenderSkinnedMeshPart(const ovrAvatarRenderPart_SkinnedMeshRender* mesh, uint32_t visibilityMask, const glm::mat4& world, const glm::mat4& view, const glm::mat4 proj, const glm::vec3& viewPos, bool renderJoints);
            void RenderSkinnedMeshPartPBS(const ovrAvatarRenderPart_SkinnedMeshRenderPBS* mesh, uint32_t visibilityMask, const glm::mat4& world, const glm::mat4& view, const glm::mat4 proj, const glm::vec3& viewPos, bool renderJoints);
            void RenderProjector(const ovrAvatarRenderPart_ProjectorRender* projector, ovrAvatar* avatar, uint32_t visibilityMask, const glm::mat4& world, const glm::mat4& view, const glm::mat4 proj, const glm::vec3& viewPos);
            void SetMeshState(GLuint program, const ovrAvatarTransform& localTransform, const MeshData* data, const ovrAvatarSkinnedMeshPose& skinnedPose, const glm::mat4& world, const glm::mat4& view, const glm::mat4 proj, const glm::vec3& viewPos);
            void ComputeWorldPose(const ovrAvatarSkinnedMeshPose& localPose, glm::mat4* worldPose);
            void SetMaterialState(GLuint program, const ovrAvatarMaterialState* state, glm::mat4* projectorInv);
            void SetTextureSampler(GLuint program, int textureUnit, const char* uniformName, ovrAvatarAssetID assetId);
            void SetTextureSamplers(GLuint program, const char* uniformName, size_t count, const int textureUnits[], const ovrAvatarAssetID assetIds[]);

        protected:
            ovrAvatar* avatar;
            Shader stdShader;
            Shader pbsShader;
            ovrID userId;
        };

        class AvatarHost {
        public:

            static AvatarHost& Instance() {
                static AvatarHost instance;
                return instance;
            }

            void RegisterAvatar(ovrID userId, Avatar* avatar);
            void HandleAvatarSpecification(const ovrAvatarMessage_AvatarSpecification* msg);
            void HandleAssetLoaded(const ovrAvatarMessage_AssetLoaded* msg);
            MeshData* LoadMesh(const ovrAvatarMeshAssetData* data);
            TextureData* LoadTexture(const ovrAvatarTextureAssetData* data);

            void* GetAsset(ovrAvatarAssetID id);

            void Update();
            void RenderAvatars(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool renderJoints);

            ovrID localUserId;
        protected:
            int loadingAssets;

            std::map<ovrID, Avatar*> userAvatarMap;
            std::map<ovrAvatarAssetID, void*> assetMap;

        };

    }

}

#endif //OMICRONRENDER_OVRAVATAR_HPP
