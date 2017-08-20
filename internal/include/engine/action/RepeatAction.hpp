//
// Created by Guy on 18/08/2017.
//

#ifndef OMICRONRENDER_REPEATACTION_HPP
#define OMICRONRENDER_REPEATACTION_HPP

#include "DelegateAction.hpp"

namespace Omicron {

    namespace Actions {

        class RepeatAction : public DelegateAction {
        public:
            RepeatAction(BaseAction* action);

            void Finish();

            virtual void Restart() override;

            int GetRepeatCount() const;

            void SetRepeatCount(int repeatCount);

        protected:
            virtual bool Delegate(OmicronEntity* entity, float delta) override;

            bool finished;
            int repeatCount;
            int executedCount;
        };

    }

}

#endif //OMICRONRENDER_REPEATACTION_HPP
