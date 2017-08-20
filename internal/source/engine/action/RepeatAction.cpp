//
// Created by Guy on 18/08/2017.
//

#include <engine/action/RepeatAction.hpp>

namespace Omicron {

    namespace Actions {

        RepeatAction::RepeatAction(BaseAction* action) : DelegateAction(action) {}

        bool RepeatAction::Delegate(OmicronEntity* entity, float delta) {
            if(executedCount == repeatCount) return true;
            if(action && action->Act(entity, delta)) {
                if(finished) return true;
                if(repeatCount > 0) executedCount++;
                if(executedCount == repeatCount) return true;
                if(action != nullptr) action->Restart();
            }
            return false;
        }

        void RepeatAction::Restart() {
            BaseAction::Restart();
            executedCount = 0;
            finished = false;
        }

        int RepeatAction::GetRepeatCount() const {
            return repeatCount;
        }

        void RepeatAction::SetRepeatCount(int repeatCount) {
            RepeatAction::repeatCount = repeatCount;
        }


    }

}