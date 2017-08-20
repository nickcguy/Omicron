//
// Created by Guy on 20/08/2017.
//

#include <mesh/SkeletalMesh.hpp>

namespace Omicron {

    void VertexBoneData::AddBoneData(unsigned int boneId, float weight) {
        for(unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
            if(Weights[i] == 0.f) {
                IDs[i] = boneId;
                Weights[i] = weight;
                return;
            }
        }
    }

    bool SkeletalMesh::LoadMesh(std::string fileName) {

        return false;
    }

    bool SkeletalMesh::InitFromScene(const aiScene* scene, std::string fileName) {
        return false;
    }

    void SkeletalMesh::LoadBones(unsigned int meshIndex, const aiMesh* mesh, std::vector<VertexBoneData>& bones) {

    }

    glm::mat4 SkeletalMesh::BoneTransform(float time, std::vector<glm::mat4>& transformations) {
        return glm::mat4();
    }

    void SkeletalMesh::ReadNodeHeirarchy(float animTime, const aiNode* node, const glm::mat4& parentTransform) {

    }

    void SkeletalMesh::CalcInterpolatedRotation(aiQuaternion& out, float animTime, const aiNodeAnim* nodeAnim) {

    }

    unsigned int SkeletalMesh::FindRotation(float animTime, const aiNodeAnim* nodeAnim) {
        return 0;
    }

}