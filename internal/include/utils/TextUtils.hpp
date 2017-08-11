//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_TEXTUTILS_HPP
#define OMICRONRENDER_TEXTUTILS_HPP

#include <string>
#include <sstream>
#include <glm/vec3.hpp>
#include <pch.hpp>
#include <vector>

namespace Omicron {

    namespace Utils {

        template <typename T> inline
        std::string ToString(T t) {
            std::stringstream ss;
            ss << t;
            return ss.str();
        }

        inline std::string Vec3ToString(glm::vec3 vec) {
            std::stringstream ss;
            ss << "["
               << vec.x
               << ", "
               << vec.y
               << ", "
               << vec.z
               << "]";
            return ss.str();
        }

        std::string TranslateGLEnum(GLenum glenum);
        void CheckErrors(std::string prefix = "");


        template<typename Out>
        inline void Split(const std::string &s, char delim, Out result) {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim))
                *(result++) = item;
        }


        inline std::vector<std::string> Split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            Split(s, delim, std::back_inserter(elems));
            return elems;
        }

    }

}

#endif //OMICRONRENDER_TEXTUTILS_HPP
