//
// Created by Guy on 17/08/2017.
//

#include <engine/action/ParallelAction.hpp>
#include <utility>

namespace Omicron {
    namespace Actions {

        ParallelAction::ParallelAction() = default;

        ParallelAction::ParallelAction(std::vector<BaseAction*> actions) : actions(std::move(actions)) {}

        bool ParallelAction::Act(OmicronEntity* entity, float delta) {
            if(complete) return true;
            complete = true;
            for(auto action : actions)
                if(!action->Act(entity, delta)) complete = false;
            return complete;
        }

        void ParallelAction::Restart() {
            complete = false;
            for(auto action : actions)
                action->Restart();
        }

    }
}