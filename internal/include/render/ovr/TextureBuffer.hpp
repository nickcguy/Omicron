//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_TEXTUREBUFFER_HPP
#define OMICRONRENDER_TEXTUREBUFFER_HPP

#include <pch.hpp>
#include <OVR_CAPI_GL.h>
#include <Extras/OVR_Math.h>
#include <cassert>

namespace Omicron {

    namespace ovr {

        using namespace OVR;

        struct DepthBuffer
        {
            GLuint        texId;

            DepthBuffer(Sizei size, int sampleCount)
            {
                UNREFERENCED_PARAMETER(sampleCount);

                assert(sampleCount <= 1); // The code doesn't currently handle MSAA textures.

                glGenTextures(1, &texId);
                glBindTexture(GL_TEXTURE_2D, texId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                GLenum internalFormat = GL_DEPTH_COMPONENT24;
                GLenum type = GL_UNSIGNED_INT;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.w, size.h, 0, GL_DEPTH_COMPONENT, type, NULL);
            }
            ~DepthBuffer()
            {
                if (texId)
                {
                    glDeleteTextures(1, &texId);
                    texId = 0;
                }
            }
        };

        struct TextureBuffer
        {
            ovrSession           Session;
            ovrTextureSwapChain  TextureChain;
            std::vector<GLuint>  texIds;
            GLuint               fboId;
            Sizei                texSize;
            int                  texCount;

            TextureBuffer(ovrSession session, bool rendertarget, bool displayableOnHmd, Sizei size, int mipLevels, unsigned char * data, int sampleCount, int texCount = 1) :
            Session(session),
            TextureChain(nullptr),
            texIds(),
            fboId(0),
            texSize(size)
            {
                UNREFERENCED_PARAMETER(sampleCount);

                assert(sampleCount <= 1); // The code doesn't currently handle MSAA textures.

                this->texCount = texCount = (displayableOnHmd ? 1 : texCount);
                this->texIds.resize(this->texCount);

                if (displayableOnHmd) {
                    // This texture isn't necessarily going to be a rendertarget, but it usually is.
                    assert(session); // No HMD? A little odd.
                    assert(sampleCount == 1); // ovr_CreateSwapTextureSetD3D11 doesn't support MSAA.

                    ovrTextureSwapChainDesc desc = {};
                    desc.Type = ovrTexture_2D;
                    desc.ArraySize = 1;
                    desc.Width = size.w;
                    desc.Height = size.h;
                    desc.MipLevels = 1;
                    desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
                    desc.SampleCount = 1;
                    desc.StaticImage = ovrFalse;

                    ovrResult result = ovr_CreateTextureSwapChainGL(Session, &desc, &TextureChain);

                    int length = 0;
                    ovr_GetTextureSwapChainLength(session, TextureChain, &length);

                    if(OVR_SUCCESS(result))
                    {
                        for (int i = 0; i < length; ++i)
                        {
                            GLuint chainTexId;
                            ovr_GetTextureSwapChainBufferGL(Session, TextureChain, i, &chainTexId);
                            glBindTexture(GL_TEXTURE_2D, chainTexId);

                            if (rendertarget)
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            }
                            else
                            {
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            }
                        }
                    }
                }else{
                    for(int i = 0; i < texCount; i++) {
                        glGenTextures(1, &texIds[i]);
                        glBindTexture(GL_TEXTURE_2D, texIds[i]);

                        if(rendertarget) {
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        } else {
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        }

                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texSize.w, texSize.h, 0, GL_RGBA,
                                     GL_FLOAT, data);

                        if (mipLevels > 1)
                            glGenerateMipmap(GL_TEXTURE_2D);

                    }
                }

                glGenFramebuffers(1, &fboId);
            }

            ~TextureBuffer()
            {
                if (TextureChain)
                {
                    ovr_DestroyTextureSwapChain(Session, TextureChain);
                    TextureChain = nullptr;
                }
                glDeleteTextures(texCount, texIds.data());
                if (fboId)
                {
                    glDeleteFramebuffers(1, &fboId);
                    fboId = 0;
                }
            }

            Sizei GetSize() const
            {
                return texSize;
            }

            void SetAndClearRenderSurface(DepthBuffer* dbuffer, bool clearDepthBuffer = true)
            {
                GLuint curTexId;
                glBindFramebuffer(GL_FRAMEBUFFER, fboId);

                if (TextureChain) {
                    int curIndex;
                    ovr_GetTextureSwapChainCurrentIndex(Session, TextureChain, &curIndex);
                    ovr_GetTextureSwapChainBufferGL(Session, TextureChain, curIndex, &curTexId);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
                    for(int i = 1; i < texCount; i++)
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texIds[i], 0);
                }else{
                    for(int i = 0; i < texCount; i++)
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texIds[i], 0);
                }

                if(dbuffer) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dbuffer->texId, 0);

                glViewport(0, 0, texSize.w, texSize.h);
                glClearColor(0.f, 0.f, 0.f, 1.f);
                if(clearDepthBuffer)
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                else
                    glClear(GL_COLOR_BUFFER_BIT);
                glEnable(GL_FRAMEBUFFER_SRGB);

                glEnable(GL_DEPTH_TEST);
                glEnable(GL_BLEND);
                glEnable(GL_CULL_FACE);

                glCullFace(GL_BACK);
                glFrontFace(GL_CW);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            void UnsetRenderSurface() {
                glBindFramebuffer(GL_FRAMEBUFFER, fboId);
                for(int i = 0; i < texCount; i++)
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
            }

            void Commit()
            {
                if (TextureChain)
                {
                    ovr_CommitTextureSwapChain(Session, TextureChain);
                }
            }
        };

    }

}

#endif //OMICRONRENDER_TEXTUREBUFFER_HPP
