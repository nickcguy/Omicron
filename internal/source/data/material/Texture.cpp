//
// Created by Guy on 07/08/2017.
//

#include <data/material/Texture.hpp>
#include <stb_image.h>
#include <iostream>
#include <mutex>

namespace Omicron {

    Texture::Texture() {
        glGenTextures(1, &this->id);

        this->wrapS = this->wrapT = GL_REPEAT;
        this->filterMin = GL_LINEAR_MIPMAP_LINEAR;
        this->filterMag = GL_LINEAR;
        this->internalFormat = GL_RGB;
        this->imageFormat = GL_RGB;
        Generate(1, 1, NULL);
    }

    void Texture::LoadFromFile(std::string filePath, GLboolean alpha) {

        if(alpha) {
            this->internalFormat = GL_RGBA;
            this->imageFormat = GL_RGBA;
        }

        int width, height, components;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &components, alpha ? 4 : 3);
        if(data) {
            std::lock_guard<std::mutex> guard(imageDataMutex);
            imageWidth = width;
            imageHeight = height;
            imageData = data;
            dirty.store(true);
        }else{
            std::cerr << "Failed to load image at: " << filePath << std::endl;
        }
    }

    Texture::~Texture() {
        glDeleteTextures(1, &this->id);
    }

    void Texture::Generate(GLuint width, GLuint height, void* data) {
        this->width = width;
        this->height = height;

        glBindTexture(GL_TEXTURE_2D, this->id);
//        SetData(width, height, data, false, GL_UNSIGNED_BYTE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMag);

//        glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
        SetData(width, height, data, false, GL_UNSIGNED_BYTE);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::SetData(GLuint width, GLuint height, void* data, bool bind, GLenum dataType) {
        if(bind) glBindTexture(GL_TEXTURE_2D, this->id);
        glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, dataType, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        if(bind) glBindTexture(GL_TEXTURE_2D, 0);

    }

    void Texture::Bind() {
        glBindTexture(GL_TEXTURE_2D, this->id);
    }

    void Texture::Invalidate() {
        if(dirty.load()) {
            std::lock_guard<std::mutex> guard(imageDataMutex);
            SetData(imageWidth, imageHeight, imageData, true);
            stbi_image_free(imageData);
            dirty.store(false);
        }
    }

}