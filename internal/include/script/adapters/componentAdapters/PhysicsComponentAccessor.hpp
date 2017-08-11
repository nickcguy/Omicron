//
// Created by Guy on 11/08/2017.
//

#ifndef OMICRONRENDER_PHYSICSCOMPONENTACCESSOR_HPP
#define OMICRONRENDER_PHYSICSCOMPONENTACCESSOR_HPP

#include <engine/component/PhysicsComponent.hpp>
#include <script/IScriptable.hpp>

namespace Omicron {

    class PhysicsComponentAccessor {
    public:
        PhysicsComponentAccessor();
        PhysicsComponentAccessor(PhysicsComponent* phys);

        void ClearForces();
        void SetGravity(glm::vec3 grav);
        glm::vec3 GetGravity();

        void ApplyCentralForce(glm::vec3 force);
        void ApplyCentralImpulse(glm::vec3 force);

        void ApplyForce(glm::vec3 force, glm::vec3 relPos);
        void ApplyImpulse(glm::vec3 force, glm::vec3 relPos);

        void ApplyTorque(glm::vec3 torque);
        void ApplyTorqueImpulse(glm::vec3 torque);

    protected:
        PhysicsComponent* phys = nullptr;
    };

    class PhysicsComponentAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(PhysicsComponentAdapter)

}

#endif //OMICRONRENDER_PHYSICSCOMPONENTACCESSOR_HPP
