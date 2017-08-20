//
// Created by Guy on 18/08/2017.
//

#include <engine/action/DelegateAction.hpp>

namespace Omicron {

    namespace Actions {

        DelegateAction::DelegateAction(BaseAction* action) : action(action) {}

        bool DelegateAction::Act(OmicronEntity* entity, float delta) {
            return Delegate(entity, delta);
        }
    }

}