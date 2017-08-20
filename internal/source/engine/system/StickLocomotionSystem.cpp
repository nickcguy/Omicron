//
// Created by Guy on 19/08/2017.
//

#include <engine/system/StickLocomotionSystem.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/system/InputSystem.hpp>

namespace Omicron {

    void StickLocomotionSystem::Update(float delta) {
        if(!inputs) {
            inputs = dynamic_cast<InputSystem*>(engine->GetSystem("InputSystem"));
            return;
        }

        delta *= 2.5f;
        if(inputs->GetInputState()->buttons->btnRThumb) {
            delta *= 2.5f;
        }

        glm::vec3 front = engine->inputProvider->GetForwardVector();
        glm::vec3 right = engine->inputProvider->GetRightVector();

        front.y = 0.f;
        right.y = 0.f;

        float x = inputs->GetInputState()->thumbsticks->thumbstickRX;
        float y = -inputs->GetInputState()->thumbsticks->thumbstickRY;

        engine->OriginPosition += ((right * x) * delta);
        engine->OriginPosition += ((front * y) * delta);
    }

    std::string StickLocomotionSystem::Name() {
        return "StickLocomotionSystem";
    }
}