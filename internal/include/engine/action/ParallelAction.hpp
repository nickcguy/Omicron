//
// Created by Guy on 17/08/2017.
//

#ifndef OMICRONRENDER_PARALLELACTION_HPP
#define OMICRONRENDER_PARALLELACTION_HPP

#include <vector>
#include "BaseAction.hpp"

namespace Omicron {
    namespace Actions {

        class ParallelAction : public BaseAction {
        public:
            ParallelAction();
            ParallelAction(std::vector<BaseAction*> actions);

            virtual bool Act(OmicronEntity* entity, float delta) override;

            virtual void Restart() override;

        protected:
            std::vector<BaseAction*> actions;
            bool complete;
        };

    }
}

#endif //OMICRONRENDER_PARALLELACTION_HPP
