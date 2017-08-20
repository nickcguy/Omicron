//
// Created by Guy on 16/08/2017.
//

#ifndef OMICRONRENDER_BASEACTION_HPP
#define OMICRONRENDER_BASEACTION_HPP

namespace Omicron {

    class OmicronEntity;

    namespace Actions {

        class BaseAction {
        public:
            virtual bool Act(OmicronEntity* entity, float delta)=0;
            virtual void Restart();

        };

    }

}

#endif //OMICRONRENDER_BASEACTION_HPP
