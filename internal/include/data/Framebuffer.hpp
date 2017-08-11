//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_FRAMEBUFFER_HPP
#define OMICRONRENDER_FRAMEBUFFER_HPP

#include <pch.hpp>
#include <render/shader/Shader.hpp>
#include <vector>

namespace Omicron {

    struct Attachment {
        int width;
        int height;
        unsigned int internalFormat;
        unsigned int imageFormat;
        unsigned int texId;
    };

    class Framebuffer {
    public:
        Framebuffer();
        void Init();

        Attachment AddAttachment(Attachment attachment);
        void InitRenderBuffer(int width, int height);

        void Resize(int width, int height);
        virtual void Bind();
        void Unbind();

        Attachment GenerateAttachmentTexture(unsigned int internalType, unsigned int imageType, int width, int height);
        Attachment& GetAttachment(int index);
        Attachment& RBO();

        unsigned int GetID();

        int width, height;

    protected:
        std::vector<Attachment> attachments;
        GLuint fbo;
        Attachment RBOAttachment;
    };

    class ShadedFramebuffer : public Framebuffer {
    public:
        Shader& GetShader();
        void SetShader(Shader shader);

        virtual void Bind() override;

    protected:
        Shader shader;
    };

}

#endif //OMICRONRENDER_FRAMEBUFFER_HPP
