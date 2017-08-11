//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_TEXTURE_HPP
#define OMICRONRENDER_TEXTURE_HPP

#include <pch.hpp>
#include <string>
#include <atomic>
#include <mutex>

namespace Omicron {

    class Texture {
    public:
        GLuint id;
        GLuint width;
        GLuint height;
        GLuint internalFormat;
        GLuint imageFormat;
        GLuint wrapS;
        GLuint wrapT;
        GLuint filterMin;
        GLuint filterMag;

        Texture();
        ~Texture();
        void LoadFromFile(std::string filePath, GLboolean alpha = GL_TRUE);
        void Generate(GLuint width, GLuint height, void* data);
        void SetData(GLuint width, GLuint height, void* data, bool bind = true, GLenum dataType = GL_UNSIGNED_BYTE);
        void Bind();
        void Invalidate();

        std::mutex imageDataMutex;
        unsigned int imageWidth, imageHeight;
        void* imageData;
        std::atomic_bool dirty;
    };

}

#endif //OMICRONRENDER_TEXTURE_HPP
