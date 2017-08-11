//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_OVRINPUTPROVIDER_HPP
#define OMICRONRENDER_OVRINPUTPROVIDER_HPP

#include <OVR_CAPI.h>
#include "InputProvider.hpp"

namespace Omicron {

    class OVRInputProvider : public InputProvider {
    public:
        virtual InputProviderType GetType() override {
            return OVR;
        }

        ovrInputState inputData = {};
        ovrTrackingState trackingState = {};

        glm::vec3 translationOffset;
    };

}

#endif //OMICRONRENDER_OVRINPUTPROVIDER_HPP
