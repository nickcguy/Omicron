//
// Created by Guy on 18/08/2017.
//

#ifndef OMICRONRENDER_DELEGATEACTION_HPP
#define OMICRONRENDER_DELEGATEACTION_HPP

#include "BaseAction.hpp"

namespace Omicron {

    namespace Actions {

        class DelegateAction : public BaseAction {
        public:
            DelegateAction(BaseAction* action);

            virtual bool Act(OmicronEntity* entity, float delta) override;

        protected:
            virtual bool Delegate(OmicronEntity* entity, float delta)=0;


            BaseAction* action;

        };

    }

}

#endif //OMICRONRENDER_DELEGATEACTION_HPP
