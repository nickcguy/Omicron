//
// Created by Guy on 08/08/2017.
//

#include <engine/system/PsychicInteractionSystem.hpp>
#include <engine/entity/OmicronEntity.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/input/OVRInputProvider.hpp>
#include <utils/MathsUtils.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/component/PhysicsComponent.hpp>

namespace Omicron {

    PyschicInteractionSystem::PyschicInteractionSystem(PhysicsSystem* physicsSystem) : physicsSystem(physicsSystem) {}

    OmicronEntity* PyschicInteractionSystem::GetFocusedEntity(int hand, float rangeScale, glm::vec3* hitPos) {

        if(engine->inputProvider->GetType() != OVR) return nullptr;

        OVRInputProvider* inputProvider = static_cast<OVRInputProvider*>(engine->inputProvider);

        OVR::Posef pose = inputProvider->trackingState.HandPoses[hand].ThePose;

        glm::vec3 direction = Utils::ComputeForwardVector(Utils::ConvertQuat(pose.Rotation));
        direction *= rangeScale;

        glm::vec3 position = Utils::ConvertVec3(pose.Translation);
        position += inputProvider->translationOffset;

        btVector3 start(position.x, position.y, position.z);
        btVector3 end(pose.Translation.x - direction.x, pose.Translation.y - direction.y, pose.Translation.z - direction.z);

        printf("Raytrace: Start [%f, %f, %f], End [%f, %f, %f]\n", start.x(), start.y(), start.z(), end.x(), end.y(), end.z());

        btCollisionWorld::ClosestRayResultCallback callback(start, end);

        engine->debugLines.push_back(new DebugLine(
                {start.x(), start.y(), start.z()}, // World start
                {end.x(), end.y(), end.z()}, // World end
                {1.f, 0.f, 0.f},
                {0.f, 1.f, 0.f}));

        physicsSystem->GetWorld()->rayTest(start, end, callback);

        if(callback.hasHit()) {

            if(hitPos) {
                hitPos->x = callback.m_hitPointWorld.x();
                hitPos->y = callback.m_hitPointWorld.y();
                hitPos->z = callback.m_hitPointWorld.z();
            }

            void* ptr = callback.m_collisionObject->getUserPointer();
            if(PhysicsComponent* physicsComp = reinterpret_cast<PhysicsComponent*>(ptr)) {
                physicsComp->GetBody()->activate(true);
                return physicsComp->owningEntity;
            }
        }

        return nullptr;
    }

    void PyschicInteractionSystem::Update(float delta) {
        UpdateHand(ovrHand_Left, delta);
        UpdateHand(ovrHand_Right, delta);
    }

    void PyschicInteractionSystem::UpdateHand(int hand, float delta) {
        if(engine->inputProvider->GetType() != OVR) return;
        OVRInputProvider* inputProvider = static_cast<OVRInputProvider*>(engine->inputProvider);
        float depression = inputProvider->inputData.HandTrigger[hand];
        if(depression > 0.1f) {
            glm::vec3 hitPos;
            OmicronEntity* entity = GetFocusedEntity(hand, 100.f, &hitPos);
            if(!entity) return;
            glm::vec3 to = Utils::ConvertVec3(inputProvider->trackingState.HandPoses[hand].ThePose.Position);
            Pull(entity, to, hitPos, depression);
        }
    }

    void PyschicInteractionSystem::Pull(OmicronEntity* entity, glm::vec3 to, glm::vec3 hit, float strength) {
        if(!entity->HasComponent<PhysicsComponent>()) return;
        PhysicsComponent* comp = entity->GetCastComponent<PhysicsComponent>();

        glm::vec3 force(to);
        glm::vec3 from = entity->transform.Translation;
        force -= from;
        force = glm::normalize(force);

        force *= strength;
        comp->GetBody()->applyForce({force.x, force.y, force.z}, {hit.x, hit.y, hit.z});
    }

    string PyschicInteractionSystem::Name() {
        return "Physics interaction";
    }

}