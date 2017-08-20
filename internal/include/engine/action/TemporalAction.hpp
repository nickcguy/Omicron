//
// Created by Guy on 17/08/2017.
//

#ifndef OMICRONRENDER_TEMPORALACTION_HPP
#define OMICRONRENDER_TEMPORALACTION_HPP

#include <data/Interpolation.hpp>
#include "BaseAction.hpp"

namespace Omicron {
    namespace Actions {

        class TemporalAction : public BaseAction {
        public:
            TemporalAction();
            TemporalAction(float duration);
            TemporalAction(float duration, Interpolation* interpolation);

            virtual bool Act(OmicronEntity* entity, float delta) override;

        protected:

            void Begin();
            void End();
            virtual void Update(float percent)=0;
            void Finish();

        public:
            virtual void Restart() override;

        protected:
            void Reset();

            Interpolation* interpolation;
            float duration;
            float time;
            bool reverse;
            bool began;
            bool complete;

        };

    }
}

#endif //OMICRONRENDER_TEMPORALACTION_HPP
