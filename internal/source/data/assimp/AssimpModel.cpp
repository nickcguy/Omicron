//
// Created by Guy on 10/08/2017.
//

#include <vector>
#include <render/shader/Shader.hpp>
#include <data/assimp/AssimpModel.hpp>

using namespace Assimp;

namespace Omicron {

    using namespace MeshFactory;

        ModelComponent::ModelComponent(std::string path) {
            LoadModel(path);
        }

        void ModelComponent::LoadModel(std::string path) {
            Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));
            ProcessNode(scene->mRootNode, scene);
        }

        void ModelComponent::ProcessNode(aiNode* node, const aiScene* scene) {
            unsigned int i;

            for(i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(ProcessMesh(mesh, scene));
            }

            for(i = 0; i < node->mNumChildren; i++)
                ProcessNode(node->mChildren[i], scene);
        }

        MeshComponent* ModelComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

            unsigned int i;
            // TODO support textures

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            for(i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;

                glm::vec3 vec;
                vec.x = mesh->mVertices[i].x;
                vec.y = mesh->mVertices[i].y;
                vec.z = mesh->mVertices[i].z;
                vertex.Position = vec;

                vec.x = mesh->mNormals[i].x;
                vec.y = mesh->mNormals[i].y;
                vec.z = mesh->mNormals[i].z;
                vertex.Normal = vec;

                if(mesh->mTextureCoords[0]) {
                    glm::vec2 uv;
                    uv.x = mesh->mTextureCoords[0][i].x;
                    uv.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoord = uv;
                }else vertex.TexCoord = {0.f, 0.f};

                if(mesh->HasVertexColors(0)) {
                    glm::vec4 colour;
                    colour.r = mesh->mColors[0]->r;
                    colour.g = mesh->mColors[0]->g;
                    colour.b = mesh->mColors[0]->b;
                    colour.a = mesh->mColors[0]->a;
                    vertex.Colour = colour;
                }else vertex.Colour = {1.f, 1.f, 1.f, 1.f};

                vertices.push_back(vertex);
            }

            unsigned int j;
            for(i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for(j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }


            return new MeshComponent(vertices, indices);
        }

        std::vector<Texture>
        ModelComponent::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
            return std::vector<Texture>();
        }

    OmicronComponent* ModelComponent::Clone() {
        return new ModelComponent(this);
    }

    ModelComponent::ModelComponent(ModelComponent* other) {
        this->directory = other->directory;
        int size = other->meshes.size();
        this->meshes.resize(size);
        for(int i = 0; i < size; i++)
            this->meshes[i] = new MeshComponent(other->meshes[i]);
    }

    std::string ModelComponent::Name() {
        return "Model";
    }


}