//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_WORLDLOADER_HPP
#define OMICRONRENDER_WORLDLOADER_HPP

#include <string>
#include <tinyxml2.h>
#include <map>
#include <render/newRender/BaseRenderer.hpp>
#include "ComponentFactory.hpp"

using namespace tinyxml2;

namespace Omicron {

    class OmicronEngine;
    class OmicronEntity;
    class Transform;

    class EngineLoader {
    public:

        static void SetRenderer(BaseRenderer* targetRenderer);

        static IComponentFactory* GetComponentFactory(std::string name);
        static void AddComponentFactory(IComponentFactory* fac);
        static void LoadIntoEngine(std::string file, OmicronEngine* engine, std::function<void(void)> callback);

        static std::string Attribute(XMLElement* element, std::string name);

    protected:


        static void LoadWorld(XMLElement* element, OmicronEngine* engine);
        static void LoadSystems(XMLElement* element, OmicronEngine* engine);
        static void LoadTemplates(XMLElement* element, OmicronEngine* engine);
        static void LoadEntities(XMLElement* element, OmicronEngine* engine);

        static void LoadSystem(XMLElement* element, OmicronEngine* engine);
        static void LoadTemplate(XMLElement* element, OmicronEngine* engine);
        static void LoadEntity(XMLElement* element, OmicronEngine* engine);
        static void LoadTransform(XMLElement* element, OmicronEntity* entity);
        static void LoadComponent(XMLElement* element, OmicronEntity* entity, OmicronEngine* engine);
    };

}

#endif //OMICRONRENDER_WORLDLOADER_HPP
