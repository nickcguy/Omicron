//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_OVRINPUTPROVIDER_HPP
#define OMICRONRENDER_OVRINPUTPROVIDER_HPP

#include <OVR_CAPI.h>
#include "InputProvider.hpp"
#include <glm/glm.hpp>

namespace Omicron {

    class OVRInputProvider : public InputProvider {
    public:
        virtual InputProviderType GetType() override {
            return OVR;
        }

        ovrInputState inputData = {};
        ovrTrackingState trackingState = {};

        glm::vec3 translationOffset;
        glm::vec3 forwardVector;
        glm::vec3 upVector;
        glm::vec3 rightVector;

        virtual glm::vec3 GetForwardVector() override {
            return forwardVector;
        }

        virtual glm::vec3 GetUpVector() override {
            return upVector;
        }

        virtual glm::vec3 GetRightVector() override {
            return rightVector;
        }

    };

}

#endif //OMICRONRENDER_OVRINPUTPROVIDER_HPP
