//
// Created by Guy on 12/08/2017.
//

#ifndef OMICRONRENDER_COMPONENTFACTORY_HPP
#define OMICRONRENDER_COMPONENTFACTORY_HPP

#include <string>
#include <tinyxml2.h>

#define REGISTER_COMPONENT_FACTORY(cls) namespace { ComponentFactory factory_##cls = ComponentFactory(new cls); }

namespace Omicron {

    class OmicronEntity;
    class OmicronEngine;

    class IComponentFactory {
    public:
        virtual void Load(tinyxml2::XMLElement* element, OmicronEntity* entity, OmicronEngine* engine)=0;
        virtual std::string Name()=0;
    };

    struct ComponentFactory {
        explicit ComponentFactory(IComponentFactory* fac);
    };

}

#endif //OMICRONRENDER_COMPONENTFACTORY_HPP
