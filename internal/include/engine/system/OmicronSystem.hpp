//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_OMICRONSYSTEM_HPP
#define OMICRONRENDER_OMICRONSYSTEM_HPP

#include <string>

namespace Omicron {

    class OmicronEngine;

    class OmicronSystem {
    public:
        OmicronSystem();
        virtual void Destroy();

        virtual void Update(float delta)=0;

        void SetEngine(OmicronEngine* engine);

        bool IsActive() const;

        void SetActive(bool active);

        virtual std::string Name()=0;

    protected:
        bool active = true;
        OmicronEngine* engine = nullptr;
    };

}

#endif //OMICRONRENDER_OMICRONSYSTEM_HPP
