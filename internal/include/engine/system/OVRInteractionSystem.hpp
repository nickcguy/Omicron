//
// Created by Guy on 20/08/2017.
//

#ifndef OMICRONRENDER_OVRINTERACTIONSYSTEM_HPP
#define OMICRONRENDER_OVRINTERACTIONSYSTEM_HPP

#include <engine/input/OVRInputProvider.hpp>
#include "OmicronSystem.hpp"
#include "InputSystem.hpp"
#include <engine/input/OVRInputProvider.hpp>

namespace Omicron {

    class OVRInteractionSystem : public OmicronSystem {
    public:
        virtual void Update(float delta) override;
        void UpdateHand(float delta, ovrHandType hand);

        virtual std::string Name() override;

    protected:
        OVRInputProvider* inputProvider = nullptr;
        InputSystem* inputs = nullptr;
    };

}

#endif //OMICRONRENDER_OVRINTERACTIONSYSTEM_HPP
