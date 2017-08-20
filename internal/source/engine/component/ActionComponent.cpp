//
// Created by Guy on 17/08/2017.
//

#include <engine/component/ActionComponent.hpp>

namespace Omicron {


    ActionComponent::ActionComponent() = default;

    ActionComponent::ActionComponent(ActionComponent* other) : action(other->action) {}

    OmicronComponent* ActionComponent::Clone() {
        return new ActionComponent(this);
    }

    std::string ActionComponent::Name() {
        return "ActionComponent";
    }
}