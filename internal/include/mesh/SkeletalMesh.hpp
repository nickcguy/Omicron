//
// Created by Guy on 20/08/2017.
//

#ifndef OMICRONRENDER_SKELETALMESH_HPP
#define OMICRONRENDER_SKELETALMESH_HPP

#include <string>
#include <assimp/scene.h>
#include <vector>
#include <glm/glm.hpp>

namespace Omicron {

    #define NUM_BONES_PER_VERTEX 100

    struct VertexBoneData {
        unsigned int IDs[NUM_BONES_PER_VERTEX];
        float Weights[NUM_BONES_PER_VERTEX];

        void AddBoneData(unsigned int boneId, float weight);
    };

    class SkeletalMesh {
    public:
        bool LoadMesh(std::string fileName);
        bool InitFromScene(const aiScene* scene, std::string fileName);
        void LoadBones(unsigned int meshIndex, const aiMesh* mesh, std::vector<VertexBoneData>& bones);
        glm::mat4 BoneTransform(float time, std::vector<glm::mat4>& transformations);
        void ReadNodeHeirarchy(float animTime, const aiNode* node, const glm::mat4& parentTransform);
        void CalcInterpolatedRotation(aiQuaternion& out, float animTime, const aiNodeAnim* nodeAnim);
        unsigned int FindRotation(float animTime, const aiNodeAnim* nodeAnim);
    };

}

#endif //OMICRONRENDER_SKELETALMESH_HPP
