//
// Created by Guy on 15/08/2017.
//

#include "QtRenderer.hpp"
#include <render/OpenGLContext.hpp>

QtRenderer::QtRenderer(Omicron::OpenGLContext* context) : FlatRenderer(context) {}

void QtRenderer::BlitToScreen(int targetFBO) {
    Omicron::FlatRenderer::BlitToScreen(fboId);

    unsigned char* data;
    glBindTexture(GL_TEXTURE_2D, texId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &data);
    int elements = sizeof(data) / sizeof(unsigned char);
    imageData.clear();
    imageData.resize(elements);
    for(int i = 0; i < elements; i++)
        imageData[i] = data[i];
    glBindTexture(GL_TEXTURE_2D, 0);
}

void QtRenderer::Init() {
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, context->GetWidth(), context->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("[ERROR] Failed to create framebuffer for QT rendering\n");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Omicron::FlatRenderer::Init();
}
