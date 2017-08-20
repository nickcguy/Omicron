//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_RENDERCOMMAND_HPP
#define OMICRONRENDER_RENDERCOMMAND_HPP

#include <glm/glm.hpp>
#include <data/material/OmicronMaterial.hpp>

namespace Omicron {

    enum RenderCommandType {
        VERTEX,
        INDEXED,
        POINTCLOUD,
        DECAL,
        RenderCommandType_LENGTH
    };

    enum VertexAttribute {
        POSITION,
        NORMAL,
        TEXCOORD,
        COLOUR,
        VertexAttribute_LENGTH
    };

    struct VertexLayout {
        VertexAttribute* attributes;
        int attributeCount;
    };

    // TODO integrate material pipeline
    struct RenderCommand {
        RenderCommandType type;
        std::string material;
        bool materialDynamic;
        unsigned int materialInstance;

        unsigned int primitive;
        VertexLayout layout;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int size;
        unsigned int offset = 0;
        unsigned int indexType = 0x1405; //GL_UNSIGNED_INT;

        glm::mat4 model;

        std::map<std::string, float> uniforms;
        float alpha = 1.f;
    };

    namespace CommandFactory {

        RenderCommand BuildVertexCommand(unsigned int VAO, unsigned int VBO, glm::mat4 model, unsigned int size, unsigned int offset = 0, unsigned int primitive = 0x0004 /*GL_TRIANGLES*/);
        RenderCommand BuildIndexCommand(unsigned int VAO, unsigned int VBO, unsigned int EBO, glm::mat4 model, unsigned int size, unsigned int offset = 0, unsigned int primitive = 0x0004 /*GL_TRIANGLES*/, unsigned int indexType = 0x1405 /*GL_UNSIGNED_INT*/);
        RenderCommand BuildPointCloud(unsigned int VAO, unsigned int VBO, glm::mat4 model, unsigned int size, unsigned int offset = 0, unsigned int primitive = 0x0000 /*GL_POINTS*/);

    }

}

#endif //OMICRONRENDER_RENDERCOMMAND_HPP
