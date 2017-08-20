//
// Created by Guy on 19/08/2017.
//

#ifndef OMICRONRENDER_BASELOCOMOTIONSYSTEM_HPP
#define OMICRONRENDER_BASELOCOMOTIONSYSTEM_HPP

#include "OmicronSystem.hpp"

namespace Omicron {

    class InputSystem;

    class StickLocomotionSystem : public OmicronSystem {

    public:
        virtual void Update(float delta) override;

        virtual std::string Name() override;

    protected:
        InputSystem* inputs = nullptr;
    };

}

#endif //OMICRONRENDER_BASELOCOMOTIONSYSTEM_HPP
