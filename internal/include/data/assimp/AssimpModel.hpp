//
// Created by Guy on 10/08/2017.
//

#ifndef OMICRONRENDER_ASSIMPMESH_HPP
#define OMICRONRENDER_ASSIMPMESH_HPP

#include <vector>
#include <render/shader/Shader.hpp>
#include "AssimpData.hpp"
#include <engine/mesh/MeshFactory.hpp>
#include <engine/component/OmicronComponent.hpp>
#include <engine/component/MeshComponent.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Omicron {

    using namespace MeshFactory;


        class ModelComponent : public OmicronComponent {
        public:
            explicit ModelComponent(std::string path);
            explicit ModelComponent(ModelComponent* other);

            virtual OmicronComponent* Clone() override;

            std::vector<MeshComponent*> meshes;

            virtual std::string Name() override;

        protected:
            std::string directory;

            void LoadModel(std::string path);
            void ProcessNode(aiNode* node, const aiScene* scene);
            MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

        };


}

#endif //OMICRONRENDER_ASSIMPMESH_HPP
