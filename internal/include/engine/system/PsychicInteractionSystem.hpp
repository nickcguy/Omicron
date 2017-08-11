//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_PSYCHICINTERACTIONSYSTEM_HPP
#define OMICRONRENDER_PSYCHICINTERACTIONSYSTEM_HPP

#include <glm/vec3.hpp>
#include "OmicronSystem.hpp"
#include "PhysicsSystem.hpp"

namespace Omicron {

    class OmicronEntity;

    class PyschicInteractionSystem : public OmicronSystem {
    public:

        PyschicInteractionSystem(PhysicsSystem* physicsSystem);

        OmicronEntity* GetFocusedEntity(int hand, float rangeScale = 1.f, glm::vec3* hitPos = nullptr);
        virtual void Update(float delta) override;

        void UpdateHand(int hand, float delta);
        void Pull(OmicronEntity* entity, glm::vec3 to, glm::vec3 hit, float strength);

        virtual std::string Name() override;

    protected:
        PhysicsSystem* physicsSystem;

    };

}

#endif //OMICRONRENDER_PSYCHICINTERACTIONSYSTEM_HPP
