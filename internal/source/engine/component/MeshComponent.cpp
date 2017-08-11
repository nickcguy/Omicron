//
// Created by Guy on 06/08/2017.
//

#include <engine/component/MeshComponent.hpp>

namespace Omicron {

    using namespace MeshFactory;

    MeshComponent::MeshComponent(std::vector<Vertex> vertices) : vertices(vertices) {

        std::vector<float> flatVerts = FlattenVertexList(vertices);
        size = vertices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), flatVerts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Colour));

        glBindVertexArray(0);
    }

    MeshComponent::MeshComponent(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {
        std::vector<float> flatVerts = FlattenVertexList(vertices);
        size = indices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), flatVerts.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
        type = RenderCommandType::INDEXED;
        primitive = GL_TRIANGLE_STRIP;

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Colour));

        glBindVertexArray(0);
    }

    std::vector<float> MeshComponent::FlattenVertexList(std::vector<Vertex> vertices) {
        std::vector<float> flatVerts;
        flatVerts.resize(vertices.size() * Vertex_FloatCount);

        int index = 0;
        for(auto vertex : vertices) {
            flatVerts[index++] = vertex.Position.x;
            flatVerts[index++] = vertex.Position.y;
            flatVerts[index++] = vertex.Position.z;

            flatVerts[index++] = vertex.TexCoord.x;
            flatVerts[index++] = vertex.TexCoord.y;

            flatVerts[index++] = vertex.Normal.x;
            flatVerts[index++] = vertex.Normal.y;
            flatVerts[index++] = vertex.Normal.z;

            flatVerts[index++] = vertex.Colour.r;
            flatVerts[index++] = vertex.Colour.g;
            flatVerts[index++] = vertex.Colour.b;
            flatVerts[index++] = vertex.Colour.a;
        }

        return flatVerts;
    }

    std::vector<float> MeshComponent::ExtractVertices(glm::vec3 scale) {
        std::vector<float> verts(vertices.size()*3);

        int i = 0;
        for(Vertex vert : vertices) {
            verts[i++] = vert.Position.x * scale.x;
            verts[i++] = vert.Position.y * scale.y;
            verts[i++] = vert.Position.z * scale.z;
        }

        return verts;
    }

    MeshComponent* MeshComponent::FromPrimitive(std::string primitiveId, std::map<std::string, int> params) {
        if(primitiveId == "CUBE")
            return new MeshComponent(MeshFactory::CubeVertices());

        if(primitiveId == "SPHERE") {
            int rings = 16;
            int sectors = 16;
            if(params.find("rings") != params.end())
                rings = params["rings"];
            if(params.find("sectors") != params.end())
                sectors = params["sectors"];

            auto pair = MeshFactory::SphereVertices(rings, sectors);
            return new MeshComponent(pair.first, pair.second);
        }

        if(primitiveId == "PLANE") {

        }
    }

    MeshComponent* MeshComponent::FromPath(std::string path) {
        return nullptr;
    }

    MeshComponent::MeshComponent(MeshComponent* other) {
        type = other->type;
        VAO = other->VAO;
        VBO = other->VBO;
        EBO = other->EBO;
        size = other->size;
        offset = other->offset;
        primitive = other->primitive;
        vertices = other->vertices;
        indices = other->indices;
    }

    OmicronComponent* MeshComponent::Clone() {
        return new MeshComponent(this);
    }

    std::string MeshComponent::Name() {
        return "MeshComponent";
    }

};