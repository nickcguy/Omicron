//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_POINTCLOUDCOMPONENT_HPP
#define OMICRONRENDER_POINTCLOUDCOMPONENT_HPP

#include <engine/mesh/MeshFactory.hpp>
#include <pch.hpp>
#include "OmicronComponent.hpp"

namespace Omicron {

    class PointCloudComponent : public OmicronComponent {
    public:
        explicit PointCloudComponent(std::vector<MeshFactory::PointVertex> vertices);
        GLuint VAO;
        GLuint VBO;
        unsigned int size;
        unsigned int offset = 0;

        static std::vector<MeshFactory::PointVertex> RandomPointCloud(unsigned int count, float radius = 64.f);

    protected:
        std::vector<float> FlattenVertexList(std::vector<MeshFactory::PointVertex> vertices);
    };

}

#endif //OMICRONRENDER_POINTCLOUDCOMPONENT_HPP
