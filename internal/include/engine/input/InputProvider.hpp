//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_INPUTPROVIDER_HPP
#define OMICRONRENDER_INPUTPROVIDER_HPP

#include <glm/vec3.hpp>

namespace Omicron {

    enum InputProviderType {
        NONE,
        KBM,
        OVR,
        InputProviderType_LENGTH
    };

    class InputProvider {
    public:
        inline virtual InputProviderType GetType() {
            return NONE;
        }

        virtual glm::vec3 GetForwardVector()=0;
        virtual glm::vec3 GetUpVector()=0;
        virtual glm::vec3 GetRightVector()=0;
    };

}

#endif //OMICRONRENDER_INPUTPROVIDER_HPP
