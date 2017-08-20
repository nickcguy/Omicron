//
// Created by Guy on 16/08/2017.
//

#ifndef OMICRONRENDER_ACTIONCOMPONENT_HPP
#define OMICRONRENDER_ACTIONCOMPONENT_HPP

#include <engine/action/BaseAction.hpp>
#include "OmicronComponent.hpp"

namespace Omicron {

    class ActionComponent : public OmicronComponent {
    public:
        ActionComponent();
        ActionComponent(ActionComponent* action);

        virtual OmicronComponent* Clone() override;
        virtual std::string Name() override;

        Actions::BaseAction* action;
    };

}

#endif //OMICRONRENDER_ACTIONCOMPONENT_HPP
