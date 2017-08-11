//
// Created by Guy on 06/08/2017.
//

#include <engine/mesh/MeshFactory.hpp>

namespace Omicron {

    namespace MeshFactory {

        std::vector<Vertex> CubeVertices() {
            return {
                // Position                // TexCoord      // Normal                  // Colour
                {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f, -0.5f},    {1.0f, 0.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, -0.5f},    {1.0f, 1.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, -0.5f},    {1.0f, 1.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f, -0.5f},    {0.0f, 1.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f},    { 0.0f,  0.0f, -1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},

                {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 1.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f,  0.5f},    {1.0f, 0.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 1.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f,  0.5f},    {0.0f, 1.0f},    { 0.0f,  0.0f,  1.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},

                {{-0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f, -0.5f},    {1.0f, 1.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f},    {-1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},

                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, -0.5f},    {1.0f, 1.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f,  0.5f},    {0.0f, 0.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    { 1.0f,  0.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},

                {{-0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f,  0.5f},    {1.0f, 0.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f, -0.5f},    {1.0f, 1.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f,  0.5f},    {1.0f, 0.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f},    {0.0f, 1.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f},    { 0.0f, -1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},

                {{-0.5f,  0.5f, -0.5f},    {0.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, -0.5f},    {1.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f,  0.5f},    {1.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f,  0.5f},    {0.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f, -0.5f},    {0.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}}
            };
        }

        std::vector<Vertex> PlaneVertices() {
            return {
                {{-0.5f, -0.5f, 0.f},    {0.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f, -0.5f, 0.f},    {1.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, 0.f},    {1.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{ 0.5f,  0.5f, 0.f},    {1.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f,  0.5f, 0.f},    {0.0f, 1.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}},
                {{-0.5f, -0.5f, 0.f},    {0.0f, 0.0f},    { 0.0f,  1.0f,  0.0f},     {1.0f, 1.0f, 1.0f, 1.0f}}
            };
        }

        std::pair<std::vector<Vertex>, std::vector<unsigned int>> SphereVertices(unsigned int rings, unsigned int sectors) {
            std::vector<glm::vec3> positions;
            std::vector<glm::vec2> uv;
            std::vector<glm::vec3> normals;
            std::vector<unsigned int> indices;

            const unsigned int X_SEGMENTS = sectors;
            const unsigned int Y_SEGMENTS = rings;
            const float PI = M_PI;
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    float yPos = std::cos(ySegment * PI);
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }

            bool oddRow = false;
            for (int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        indices.push_back(y       * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }

            std::vector<Vertex> data(positions.size());
            for (int i = 0; i < positions.size(); ++i) {
                Vertex vert = {};
                vert.Position = positions[i];
//                data.push_back(positions[i].x);
//                data.push_back(positions[i].y);
//                data.push_back(positions[i].z);
                if (uv.size() > 0)
                    vert.TexCoord = uv[i];
                if (normals.size() > 0)
                    vert.Normal = normals[i];

                vert.Colour = {1.0f, 1.0f, 1.0f, 1.0f};

                data[i] = vert;
            }

            return {data, indices};
        }

    }

}