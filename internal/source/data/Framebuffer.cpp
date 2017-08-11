//
// Created by Guy on 08/08/2017.
//

#include <data/Framebuffer.hpp>

namespace Omicron {

    Framebuffer::Framebuffer() = default;

    void Framebuffer::Init() {
        glGenFramebuffers(1, &fbo);
    }


    Attachment Framebuffer::AddAttachment(Attachment attachment) {
        attachments.push_back(attachment);
        return attachment;
    }

    void Framebuffer::InitRenderBuffer(int width, int height) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        unsigned int attrs[attachments.size()];

        for(int i = 0; i < attachments.size(); i++) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, attachments[i].texId, 0);
            attrs[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(attachments.size(), attrs);

        glGenRenderbuffers(1, &RBOAttachment.texId);
        glBindRenderbuffer(GL_RENDERBUFFER, RBOAttachment.texId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOAttachment.texId);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("Error creating framebuffer\n");
        }else{
            printf("Framebuffer created successfully\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(int width, int height) {
        Framebuffer::Bind();
        this->width = width;
        this->height = height;

        for(Attachment a : attachments) {
            glBindTexture(GL_TEXTURE_2D, a.texId);
            glTexImage2D(GL_TEXTURE_2D, 0, a.internalFormat, width, height, 0, a.imageFormat, GL_UNSIGNED_BYTE, NULL);
        }

        glBindRenderbuffer(GL_RENDERBUFFER, RBOAttachment.texId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        Unbind();
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Attachment
    Framebuffer::GenerateAttachmentTexture(unsigned int internalType, unsigned int imageType, int width, int height) {
        unsigned int texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, internalType, width, height, 0, imageType, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        return Attachment{width, height, internalType, imageType, texId};
    }

    Attachment& Framebuffer::GetAttachment(int index) {
        return attachments[index];
    }

    unsigned int Framebuffer::GetID() {
        return fbo;
    }

    Attachment& Framebuffer::RBO() {
        return RBOAttachment;
    }

    // SHADED FRAMEBUFFER

    Shader& ShadedFramebuffer::GetShader() {
        return shader;
    }

    void ShadedFramebuffer::SetShader(Shader shader) {
        this->shader = shader;
    }

    void ShadedFramebuffer::Bind() {
        Framebuffer::Bind();
        this->shader.Use();
    }

}