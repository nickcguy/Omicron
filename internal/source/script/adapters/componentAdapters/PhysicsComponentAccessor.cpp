//
// Created by Guy on 11/08/2017.
//

#include <script/adapters/componentAdapters/PhysicsComponentAccessor.hpp>


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

    void PhysicsComponentAdapter::Register(const sel::State& state) {
        state["PhysicsComponent"].SetClass<PhysicsComponentAccessor>(
        "ClearForces", &PhysicsComponentAccessor::ClearForces,
        "SetGravity", &PhysicsComponentAccessor::SetGravity,
        "GetGravity", &PhysicsComponentAccessor::GetGravity,
        "ApplyCentralForce", &PhysicsComponentAccessor::ApplyCentralForce,
        "ApplyCentralImpulse", &PhysicsComponentAccessor::ApplyCentralImpulse,
        "ApplyForce", &PhysicsComponentAccessor::ApplyForce,
        "ApplyImpulse", &PhysicsComponentAccessor::ApplyImpulse,
        "ApplyTorque", &PhysicsComponentAccessor::ApplyTorque,
        "ApplyTorqueImpulse", &PhysicsComponentAccessor::ApplyTorqueImpulse
        );
    }

    const char* PhysicsComponentAdapter::Name() {
        return "PhysicsComponentAdapter";
    }
}
