//
// Created by Guy on 12/08/2017.
//

#include <io/ComponentFactory.hpp>
#include <io/EngineLoader.hpp>

namespace Omicron {

    ComponentFactory::ComponentFactory(Omicron::IComponentFactory* fac) {
        printf("Registering Component factory at runtime: %s\n", fac->Name().c_str());
        if(fac != nullptr)
            EngineLoader::AddComponentFactory(fac);
    }

}