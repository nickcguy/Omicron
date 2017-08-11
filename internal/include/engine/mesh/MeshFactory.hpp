//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_MESHFACTORY_HPP
#define OMICRONRENDER_MESHFACTORY_HPP


#include <glm/glm.hpp>
#include <vector>

namespace Omicron {

    namespace MeshFactory {


        static const int Vertex_FloatCount = 12;

        struct Vertex {
            glm::vec3 Position;
            glm::vec2 TexCoord;
            glm::vec3 Normal;
            glm::vec4 Colour;
        };

        struct PointVertex {
            glm::vec3 Position;
            glm::vec4 Colour;

            inline Vertex ToVertex() {
                return {
                Position,
                {0.f, 0.f},
                {0.f, 0.f, 0.f},
                Colour
                };
            }
        };

        std::vector<Vertex> CubeVertices();
        std::vector<Vertex> PlaneVertices();
        std::pair<std::vector<Vertex>, std::vector<unsigned int>> SphereVertices(unsigned int rings, unsigned int sectors);

    };

};

#endif //OMICRONRENDER_MESHFACTORY_HPP
