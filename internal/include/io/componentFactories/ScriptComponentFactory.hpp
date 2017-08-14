//
// Created by Guy on 12/08/2017.
//

#ifndef OMICRONRENDER_SCRIPTCOMPONENTFACTORY_HPP
#define OMICRONRENDER_SCRIPTCOMPONENTFACTORY_HPP

#include <io/ComponentFactory.hpp>

namespace Omicron {

    class ScriptComponentFactory : public IComponentFactory {
    public:
        virtual void Load(tinyxml2::XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) override;

        virtual std::string Name() override;
    };

    REGISTER_COMPONENT_FACTORY(ScriptComponentFactory)

}

#endif //OMICRONRENDER_SCRIPTCOMPONENTFACTORY_HPP
