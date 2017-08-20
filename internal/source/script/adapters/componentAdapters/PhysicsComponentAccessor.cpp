//
// Created by Guy on 11/08/2017.
//

#include <script/adapters/componentAdapters/PhysicsComponentAccessor.hpp>
#include <glm/glm.hpp>


namespace Omicron {

    PhysicsComponentAccessor::PhysicsComponentAccessor() = default;

    PhysicsComponentAccessor::PhysicsComponentAccessor(PhysicsComponent* phys) : phys(phys) {}

    void PhysicsComponentAccessor::ClearForces() {
        phys->GetBody()->clearForces();
    }

    void PhysicsComponentAccessor::SetGravity(glm::vec3 grav) {
        phys->GetBody()->setGravity(btVector3(grav.x, grav.y, grav.z));
    }

    glm::vec3 PhysicsComponentAccessor::GetGravity() {
        btVector3 grav = phys->GetBody()->getGravity();
        return glm::vec3(grav.x(), grav.y(), grav.z());
    }

    void PhysicsComponentAccessor::ApplyCentralForce(glm::vec3 force) {

    }

    void PhysicsComponentAccessor::ApplyCentralImpulse(glm::vec3 force) {

    }

    void PhysicsComponentAccessor::ApplyForce(glm::vec3 force, glm::vec3 relPos) {

    }

    void PhysicsComponentAccessor::ApplyImpulse(glm::vec3 force, glm::vec3 relPos) {

    }

    void PhysicsComponentAccessor::ApplyTorque(glm::vec3 torque) {

    }

    void PhysicsComponentAccessor::ApplyTorqueImpulse(glm::vec3 torque) {

    }

    std::vector<float> PhysicsComponentAccessor::GetWorldPositionSet() {
        auto trans = phys->GetBodyTransform();
        std::vector<float> pos(3);
        pos[0] = trans.getOrigin().x();
        pos[1] = trans.getOrigin().y();
        pos[2] = trans.getOrigin().z();
        return pos;
    }

    float PhysicsComponentAccessor::GetWorldPosition(int val) {
        return GetWorldPositionSet()[glm::clamp(val, 0, 2)];
    }

    void PhysicsComponentAdapter::Register(const sel::State& state) {
        state["PhysicsComponent"].SetClass<PhysicsComponent>(
        "ClearForces", &PhysicsComponent::ClearForces,
        "SetGravity", &PhysicsComponent::SetGravity,
        "GetGravity", &PhysicsComponent::GetGravity,
        "ApplyCentralForce", &PhysicsComponent::ApplyCentralForce,
        "ApplyCentralImpulse", &PhysicsComponent::ApplyCentralImpulse,
        "ApplyForce", &PhysicsComponent::ApplyForce,
        "ApplyImpulse", &PhysicsComponent::ApplyImpulse,
        "ApplyTorque", &PhysicsComponent::ApplyTorque,
        "ApplyTorqueImpulse", &PhysicsComponent::ApplyTorqueImpulse,
        "GetWorldPosition", &PhysicsComponent::GetWorldPosition
        );
    }

    const char* PhysicsComponentAdapter::Name() {
        return "PhysicsComponentAdapter";
    }
}
