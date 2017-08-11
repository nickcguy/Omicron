//
// Created by Guy on 08/08/2017.
//

#include <render/PrimitiveRenderer.hpp>
#include <pch.hpp>

namespace Omicron {

    void PrimitiveRenderer::RenderQuad() {
        if(quadVAO == 0) {
            float quadVertices[] = {
                -1.0f,  1.0f,  0.0f,    0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f,    0.0f, 0.0f,
                 1.0f,  1.0f,  0.0f,    1.0f, 1.0f,
                 1.0f, -1.0f,  0.0f,    1.0f, 0.0f,
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }

        glBindVertexArray(quadVAO);
        DrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    }

}