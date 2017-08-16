//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_LIGHT_HPP
#define OMICRONRENDER_LIGHT_HPP

#include <glm/vec3.hpp>

namespace Omicron {

    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOT,
        LightType_LENGTH
    };

    struct Light {
        LightType type;
        glm::vec3 position;
        glm::vec3 direction;

        glm::vec3 colour;

        float cutoff;
        float outerCutoff;

        float constant;
        float linear;
        float quadratic;
        float intensity;
    };

}

#endif //OMICRONRENDER_LIGHT_HPP
