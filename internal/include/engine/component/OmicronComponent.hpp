//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OMICRONCOMPONENT_HPP
#define OMICRONRENDER_OMICRONCOMPONENT_HPP

#include <string>

namespace Omicron {

    class OmicronEntity;

    class OmicronComponent {
    public:
        OmicronEntity* owningEntity = nullptr;

        virtual OmicronComponent* Clone()=0;

        virtual std::string Name()=0;

    };

};

#endif //OMICRONRENDER_OMICRONCOMPONENT_HPP