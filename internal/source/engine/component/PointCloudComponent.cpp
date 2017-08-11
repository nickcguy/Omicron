//
// Created by Guy on 06/08/2017.
//

#include <engine/component/PointCloudComponent.hpp>
#include <utils/MathsUtils.hpp>
#include <glm/gtc/random.hpp>

namespace Omicron {

    PointCloudComponent::PointCloudComponent(std::vector<MeshFactory::PointVertex> vertices) {
        std::vector<float> flatVerts = FlattenVertexList(vertices);
        size = vertices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshFactory::Vertex), flatVerts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshFactory::Vertex), (GLvoid*)offsetof(MeshFactory::Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshFactory::Vertex), (GLvoid*)offsetof(MeshFactory::Vertex, TexCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshFactory::Vertex), (GLvoid*)offsetof(MeshFactory::Vertex, Normal));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(MeshFactory::Vertex), (GLvoid*)offsetof(MeshFactory::Vertex, Colour));

        glBindVertexArray(0);
    }

    std::vector<float> PointCloudComponent::FlattenVertexList(std::vector<MeshFactory::PointVertex> vertices) {
        std::vector<float> flatVerts;
        flatVerts.resize(vertices.size() * MeshFactory::Vertex_FloatCount);

        int index = 0;
        for(auto vert : vertices) {
            MeshFactory::Vertex vertex = vert.ToVertex();

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

    std::vector<MeshFactory::PointVertex> PointCloudComponent::RandomPointCloud(unsigned int count, float radius) {
        std::vector<MeshFactory::PointVertex> points(count);

        float minPoint = -radius;
        float maxPoint = radius;

        for(int i = 0; i < count; ++i) {
            MeshFactory::PointVertex vertex;

            vertex.Position = glm::ballRand(radius);


//            glm::vec3 dir = glm::normalize(vertex.Position);
//
//            auto r = glm::sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
//            auto t = glm::atan(dir.y / dir.x);
//            auto p = glm::acos(dir.z / r);

            glm::vec3 v1(vertex.Position);
            glm::vec3 v2(0.f);

            float xDiff = v2.x - v1.x;
            float yDiff = v2.z - v1.z;
            auto angle = std::atan2(yDiff, xDiff) * 180.0 / M_PI;

            angle += 180.f;

            float h = angle;

            float perc = ((v1.y - minPoint) * 100) / (maxPoint - minPoint);

            auto s = (0.3f + (1.0f - 0.3f) * (perc / 100));
            auto v = 0.6f;

//            h *= 360.f;

            Utils::HSVtoRGB(vertex.Colour.r, vertex.Colour.g, vertex.Colour.b, h, s, v);

//            vertex.Colour.r = ((float)Utils::fastRand_Range(0, 255)) / 255.f;
//            vertex.Colour.g = ((float)Utils::fastRand_Range(0, 255)) / 255.f;
//            vertex.Colour.b = ((float)Utils::fastRand_Range(0, 255)) / 255.f;
            vertex.Colour.a = 1.0f;

            points[i] = vertex;
        }

        return points;
    }

}