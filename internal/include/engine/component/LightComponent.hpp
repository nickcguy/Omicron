//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_LIGHTCOMPONENT_HPP
#define OMICRONRENDER_LIGHTCOMPONENT_HPP

#include <data/Light.hpp>
#include <io/ComponentFactory.hpp>
#include <io/EngineLoader.hpp>
#include <glm/glm.hpp>
#include "OmicronComponent.hpp"
#include "../entity/OmicronEntity.hpp"

namespace Omicron {

    class LightComponent : public OmicronComponent {
    public:

        inline LightComponent(LightComponent* other) {
            this->lightData = other->lightData;
        }

        inline LightComponent(LightType type = POINT) {
            lightData.type = type;
        }

        virtual OmicronComponent* Clone() override {
            return new LightComponent(this);
        }

        virtual std::string Name() override {
            return "LightComponent";
        }

        Light lightData;
    };

    class LightComponentFactory : public IComponentFactory {
    public:
        inline virtual void Load(tinyxml2::XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) override {
            std::string strType = EngineLoader::Attribute(element, "lightType");


            LightType type = POINT;
            if(strType == "DIRECTIONAL")
                type = DIRECTIONAL;
            else if(strType == "SPOT")
                type = SPOT;


            glm::vec3 colour(1.f);
            glm::vec3 position(0.f);
            glm::vec3 direction(0.f);
            float     constant = 1.f;
            float     linear = 1.f;
            float     quadratic = 1.f;
            float     cutoff = 1.f;
            float     outerCutoff = 1.f;
            float     intensity = 100.f;

            XMLElement* property = element->FirstChildElement("Parameter");

            while(property) {
                std::string propName = EngineLoader::Attribute(property, "name");
                if(propName == "colour") {
                    property->QueryFloatAttribute("r", &colour.x);
                    property->QueryFloatAttribute("g", &colour.y);
                    property->QueryFloatAttribute("b", &colour.z);
                }else if(propName == "position") {
                    property->QueryFloatAttribute("x", &position.x);
                    property->QueryFloatAttribute("y", &position.y);
                    property->QueryFloatAttribute("z", &position.z);
                }else if(propName == "direction") {
                    property->QueryFloatAttribute("x", &direction.x);
                    property->QueryFloatAttribute("y", &direction.y);
                    property->QueryFloatAttribute("z", &direction.z);
                }else if(propName == "constant") {
                    property->QueryFloatAttribute("value", &constant);
                }else if(propName == "linear") {
                    property->QueryFloatAttribute("value", &linear);
                }else if(propName == "quadratic") {
                    property->QueryFloatAttribute("value", &quadratic);
                }else if(propName == "cutoff") {
                    property->QueryFloatAttribute("value", &cutoff);
                    bool isRad = false;
                    property->QueryBoolAttribute("radians", &isRad);
                    if(!isRad)
                        cutoff = glm::radians(cutoff);
                }else if(propName == "outercutoff") {
                    property->QueryFloatAttribute("value", &outerCutoff);
                    bool isRad = false;
                    property->QueryBoolAttribute("radians", &isRad);
                    if(!isRad)
                        outerCutoff = glm::radians(outerCutoff);
                }else if(propName == "intensity") {
                    property->QueryFloatAttribute("value", &intensity);
                }

                property = property->NextSiblingElement("Parameter");
            }

            LightComponent* comp = new LightComponent(type);
            comp->lightData.colour = colour;
            comp->lightData.position = position;
            comp->lightData.direction = direction;
            comp->lightData.constant  = constant ;
            comp->lightData.linear  = linear;
            comp->lightData.quadratic  = quadratic;
            comp->lightData.cutoff  = cutoff;
            comp->lightData.outerCutoff  = outerCutoff;
            comp->lightData.intensity = intensity;
            entity->SetComponent<LightComponent>(comp);
        }

        inline virtual std::string Name() override {
            return "LightComponent";
        }
    };

    REGISTER_COMPONENT_FACTORY(LightComponentFactory);

}

#endif //OMICRONRENDER_LIGHTCOMPONENT_HPP
