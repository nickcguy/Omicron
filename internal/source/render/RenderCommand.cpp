//
// Created by Guy on 06/08/2017.
//

#include <render/RenderCommand.hpp>

namespace Omicron {

    namespace CommandFactory {

        RenderCommand BuildVertexCommand(unsigned int VAO, unsigned int VBO, glm::mat4 model, unsigned int size, unsigned int offset, unsigned int primitive) {
            RenderCommand cmd;
            cmd.type = VERTEX;
            cmd.primitive = primitive;
            cmd.VAO = VAO;
            cmd.VBO = VBO;
            cmd.size = size;
            cmd.offset = offset;
            cmd.model = model;
            return cmd;
        }

        RenderCommand BuildIndexCommand(unsigned int VAO, unsigned int VBO, unsigned int EBO, glm::mat4 model, unsigned int size, unsigned int offset, unsigned int primitive, unsigned int indexType) {
            RenderCommand cmd;
            cmd.type = INDEXED;
            cmd.primitive = primitive;
            cmd.VAO = VAO;
            cmd.VBO = VBO;
            cmd.EBO = EBO;
            cmd.size = size;
            cmd.offset = offset;
            cmd.model = model;
            cmd.indexType = indexType;
            return cmd;
        }

        RenderCommand BuildPointCloud(unsigned int VAO, unsigned int VBO, glm::mat4 model, unsigned int size, unsigned int offset, unsigned int primitive) {
            RenderCommand cmd;
            cmd.type = POINTCLOUD;
            cmd.primitive = primitive;
            cmd.VAO = VAO;
            cmd.VBO = VBO;
            cmd.size = size;
            cmd.offset = offset;
            cmd.model = model;
            return cmd;
        }

    };

};