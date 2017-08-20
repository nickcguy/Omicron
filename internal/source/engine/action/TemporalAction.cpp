//
// Created by Guy on 17/08/2017.
//

#include <engine/action/TemporalAction.hpp>

namespace Omicron {
    namespace Actions {

        TemporalAction::TemporalAction() : TemporalAction(1.f) {}

        TemporalAction::TemporalAction(float duration) : TemporalAction(duration, new LambdaInterpolation([](float a) { return a; })) {}

        TemporalAction::TemporalAction(float duration, Interpolation* interpolation) : duration(duration), interpolation(interpolation) {}

        bool TemporalAction::Act(OmicronEntity* entity, float delta) {
            if(complete) return true;
            if(!began) {
                Begin();
                began = true;
            }
            time += delta;
            complete = time >= duration;
            float percent;
            if(complete)
                percent = 1;
            else {
                percent = time / duration;
                if(interpolation != nullptr) percent = interpolation->Apply(percent);
            }
            Update(reverse ? 1 - percent : percent);
            if(complete) End();
            return complete;
        }

        void TemporalAction::Begin() {

        }

        void TemporalAction::End() {

        }

        void TemporalAction::Finish() {

        }

        void TemporalAction::Restart() {
            BaseAction::Restart();
            time = 0;
            began = false;
            complete = false;
        }

        void TemporalAction::Reset() {
            reverse = false;
        }

    }
}