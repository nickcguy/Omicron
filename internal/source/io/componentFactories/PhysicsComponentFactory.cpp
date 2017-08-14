//
// Created by Guy on 12/08/2017.
//

#include <io/componentFactories/PhysicsComponentFactory.hpp>
#include <engine/system/OmicronSystem.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/component/PhysicsComponent.hpp>
#include <io/EngineLoader.hpp>

namespace Omicron {


    void PhysicsComponentFactory::Load(tinyxml2::XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) {
        bool match = true;
        element->QueryBoolAttribute("match", &match);
        bool kinematic = false;
        element->QueryBoolAttribute("kinematic", &kinematic);
        float mass = 0.0f;
        element->QueryFloatAttribute("mass", &mass);
        bool canSleep = true;
        element->QueryBoolAttribute("canSleep", &canSleep);

        if(!match) {
            printf("[WARN] Non-matching physics component not yet supported, forcing mesh match...\n");
            match = true;
        }

        if(match) {
            OmicronSystem* system = engine->GetSystem("Physics");
            if(!system) {
                printf("[ERROR] Unable to find physics system\n");
                return;
            }

            if(PhysicsSystem* phys = dynamic_cast<PhysicsSystem*>(system)) {
                PhysicsComponent* physicsComponent = new PhysicsComponent(phys);
                entity->SetComponent<PhysicsComponent>(physicsComponent);
                if(match)
                    physicsComponent->MatchEntityMesh();
                // TODO implement kinematic simulation
//                    if(kinematic)
//                        physicsComponent->GetBody()->setCollisionFlags(physicsComponent->GetBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                physicsComponent->AddToWorld(mass);
//                if(!canSleep)
                    physicsComponent->GetBody()->setActivationState(DISABLE_DEACTIVATION);

                tinyxml2::XMLElement* props = element->FirstChildElement("Property");
                while(props) {
                    std::string propName = EngineLoader::Attribute(props, "name");
                    if(propName == "gravity") {
                        auto worldGrav = phys->GetWorld()->getGravity();
                        float x = worldGrav.x();
                        float y = worldGrav.y();
                        float z = worldGrav.z();
                        props->QueryFloatAttribute("x", &x);
                        props->QueryFloatAttribute("y", &y);
                        props->QueryFloatAttribute("z", &z);
                        physicsComponent->GetBody()->setGravity({x, y, z});
                    }
                    props = props->NextSiblingElement("Property");
                }

            }else{
                printf("[ERROR] Unable to cast detected system to PhysicsSystem\n");
                return;
            }
        }
    }

    std::string PhysicsComponentFactory::Name() {
        return "PhysicsComponent";
    }
}