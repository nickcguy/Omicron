//
// Created by Guy on 14/08/2017.
//

#include <render/newRender/BaseRenderer.hpp>

namespace Omicron {

    BaseRenderer::BaseRenderer(OpenGLContext* context) : context(context) {}

    void BaseRenderer::Render(RenderCommand cmd) {
        glBindVertexArray(cmd.VAO);
        switch(cmd.type) {
            case VERTEX: case POINTCLOUD:
                DrawArrays(cmd.primitive, cmd.offset, cmd.size);
                break;
            case INDEXED:
                DrawElements(cmd.primitive, cmd.size, cmd.indexType, (void*) cmd.offset);
                break;
            default: break;
        }
    }

    MaterialManager& BaseRenderer::GetMtlManager() {
        return mtlManager;
    }

    void BaseRenderer::SetBufferType(int type) {
        this->bufferType = type;
    }

    unsigned int BaseRenderer::GetPolygonMode() const {
        return polygonMode;
    }

    void BaseRenderer::SetPolygonMode(unsigned int polygonMode) {
        BaseRenderer::polygonMode = polygonMode;
    }

}