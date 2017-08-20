//
// Created by Guy on 07/08/2017.
//

#include <utils/TextUtils.hpp>
#include <pch.hpp>

#ifndef GLENUM_LOC
#define GLENUM_LOC(glenum) case glenum: return #glenum
#endif

namespace Omicron {

    namespace Utils {

        std::string TranslateGLEnum(GLenum glenum) {
            switch(glenum) {
                GLENUM_LOC(GL_NO_ERROR);
                GLENUM_LOC(GL_INVALID_ENUM);
                GLENUM_LOC(GL_INVALID_VALUE);
                GLENUM_LOC(GL_INVALID_OPERATION);
                GLENUM_LOC(GL_INVALID_FRAMEBUFFER_OPERATION);
                GLENUM_LOC(GL_OUT_OF_MEMORY);
                GLENUM_LOC(GL_FRAMEBUFFER_UNDEFINED);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
                GLENUM_LOC(GL_FRAMEBUFFER_UNSUPPORTED);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
                GLENUM_LOC(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
            }

            std::stringstream ss;
            ss << glenum;
            return ss.str();
        }

        void CheckErrors(std::string prefix) {
            GLenum err;
            while((err = glGetError()) != GL_NO_ERROR) {
                printf("[DEBUG] %s: %s\n", prefix.c_str(), Utils::TranslateGLEnum(err).c_str());
            }
        }

    }

}