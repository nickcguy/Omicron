//
// Created by Guy on 12/08/2017.
//

#ifndef OMICRONRENDER_PHYSICSCOMPONENTFACTORY_HPP
#define OMICRONRENDER_PHYSICSCOMPONENTFACTORY_HPP

#include "../ComponentFactory.hpp"

namespace Omicron {

    class PhysicsComponentFactory : public IComponentFactory {
    public:
        virtual void Load(tinyxml2::XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) override;
        virtual std::string Name() override;
    };

    REGISTER_COMPONENT_FACTORY(PhysicsComponentFactory)

}

#endif //OMICRONRENDER_PHYSICSCOMPONENTFACTORY_HPP
