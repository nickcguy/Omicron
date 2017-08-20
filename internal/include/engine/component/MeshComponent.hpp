//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_MESHCOMPONENT_HPP
#define OMICRONRENDER_MESHCOMPONENT_HPP

#include <glad/glad.h>
#include <engine/mesh/MeshFactory.hpp>
#include <data/material/OmicronMaterial.hpp>
#include <render/RenderCommand.hpp>
#include "OmicronComponent.hpp"
#include "MaterialComponent.hpp"

namespace Omicron {

    class MeshComponent : public OmicronComponent {
    public:

        explicit MeshComponent(std::vector<MeshFactory::Vertex> vertices);
        explicit MeshComponent(MeshComponent* other);
        MeshComponent(std::vector<MeshFactory::Vertex> vertices, std::vector<unsigned int> indices, unsigned int primitive = GL_TRIANGLE_STRIP);

        virtual OmicronComponent* Clone() override;

        RenderCommandType type = RenderCommandType::VERTEX;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        unsigned int size;
        unsigned int offset = 0;
        unsigned int primitive = GL_TRIANGLES;

        std::vector<MeshFactory::Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<float> ExtractVertices(glm::vec3 scale = glm::vec3(1.f));

        static MeshComponent* FromPrimitive(std::string primitiveId, std::map<std::string, int> params = std::map<std::string, int>());
        static MeshComponent* FromPath(std::string path);

        virtual std::string Name() override;

        MaterialComponent* materialComponent = nullptr;

    protected:
        std::vector<float> FlattenVertexList(std::vector<MeshFactory::Vertex> vertices);
    };

};

#endif //OMICRONRENDER_MESHCOMPONENT_HPP
