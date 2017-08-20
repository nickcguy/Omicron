//
// Created by Guy on 17/08/2017.
//

#ifndef OMICRONRENDER_ACTIONSYSTEM_HPP
#define OMICRONRENDER_ACTIONSYSTEM_HPP

#include <engine/component/ActionComponent.hpp>
#include "engine/OmicronEngine.hpp"

namespace Omicron {

    class ActionSystem : public OmicronSystem {
    public:

        virtual void Update(float delta) override {
            auto actors = engine->GetEntitiesWith<ActionComponent>();
            for(auto actor : actors) {
                auto comp = actor->GetCastComponent<ActionComponent>();
                comp->action->Act(actor, delta);
            }
        }

        virtual std::string Name() override {
            return "ActionSystem";
        }
    };


}

#endif //OMICRONRENDER_ACTIONSYSTEM_HPP
