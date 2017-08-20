//
// Created by Guy on 20/08/2017.
//

#include <engine/system/OVRInteractionSystem.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/system/InputSystem.hpp>
#include <engine/component/PhysicsComponent.hpp>

namespace Omicron {


    void OVRInteractionSystem::Update(float delta) {
        if(!inputProvider) {
            inputProvider = dynamic_cast<OVRInputProvider*>(engine->inputProvider);
            return;
        }
        if(!inputs) {
            inputs = dynamic_cast<InputSystem*>(engine->GetSystem("InputSystem"));
            return;
        }

        if(inputs->GetInputState()->triggers->triggerLGrip > 0.9f)
            UpdateHand(delta, ovrHand_Left);

        if(inputs->GetInputState()->triggers->triggerRGrip > 0.9f)
            UpdateHand(delta, ovrHand_Right);

    }

    std::string OVRInteractionSystem::Name() {
        return "OVRInteractionSystem";
    }

    void OVRInteractionSystem::UpdateHand(float delta, ovrHandType hand) {
        auto handPose = inputProvider->trackingState.HandPoses[hand].ThePose;
        auto position = handPose.Position;
        const float interactionRange = 0.5f;
        auto entities = engine->GetPhysicalEntitiesInRange(EXTRACT_VECTOR(position), interactionRange);
        if(entities.empty()) return;
        auto target = entities[0];
        PhysicsComponent* phys = target->GetCastComponent<PhysicsComponent>();
        phys->ApplyCentralImpulse(glm::vec3(0.f, 1.f, 0.f));
    }
}