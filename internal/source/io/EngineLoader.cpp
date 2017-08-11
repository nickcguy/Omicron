//
// Created by Guy on 09/08/2017.
//

#include <io/EngineLoader.hpp>
#include <engine/OmicronEngine.hpp>
#include <script/ScriptHost.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/system/PsychicInteractionSystem.hpp>
#include <engine/component/MeshComponent.hpp>
#include <utils/TextUtils.hpp>
#include <engine/component/PhysicsComponent.hpp>
#include <engine/component/MaterialComponent.hpp>
#include <engine/system/MaterialSystem.hpp>
#include <data/assimp/AssimpModel.hpp>
#include <script/ScriptComponent.hpp>
#include <utils/FileUtils.hpp>

using namespace tinyxml2;

namespace Omicron {

    void EngineLoader::LoadIntoEngine(std::string file, OmicronEngine* engine) {
        XMLDocument doc;
        XMLError err = doc.LoadFile(file.c_str());
        if(err == XML_SUCCESS) {

            XMLElement* rootElement = doc.FirstChildElement("Engine");

            if(!rootElement) {
                printf("[ERROR] No root <Engine> tag\n");
                return;
            }

            DEBUG_PRINT(XMLElement* worldElement = rootElement->FirstChildElement("World"));
            DEBUG_PRINT(XMLElement* systemsElement = rootElement->FirstChildElement("Systems"));
            DEBUG_PRINT(XMLElement* templatesElement = rootElement->FirstChildElement("Templates"));
            DEBUG_PRINT(XMLElement* entitiesElement = rootElement->FirstChildElement("Entities"));

            if(worldElement)     LoadWorld(worldElement, engine);
            if(systemsElement)   LoadSystems(systemsElement, engine);
            if(templatesElement) LoadTemplates(templatesElement, engine);
            if(entitiesElement)  LoadEntities(entitiesElement, engine);

        }else{
            printf("Failed to load file \"%s\", [%s]\n", file.c_str(), doc.ErrorName());
        }
    }

    void EngineLoader::LoadWorld(XMLElement* element, OmicronEngine* engine) {
        // TODO implement world options such as skybox and atmosphere settings
    }

    void EngineLoader::LoadSystems(XMLElement* element, OmicronEngine* engine) {
        XMLElement* system = element->FirstChildElement("System");

        while(system) {
            LoadSystem(system, engine);
            system = system->NextSiblingElement("System");
        }
    }

    void EngineLoader::LoadTemplates(XMLElement* element, OmicronEngine* engine) {
        XMLElement* entity = element->FirstChildElement("Template");

        while(entity) {
            LoadTemplate(entity, engine);
            entity = entity->NextSiblingElement("Template");
        }
    }

    void EngineLoader::LoadEntities(XMLElement* element, OmicronEngine* engine) {
        XMLElement* entity = element->FirstChildElement("Entity");

        while(entity) {
            LoadEntity(entity, engine);
            entity = entity->NextSiblingElement("Entity");
        }
    }

    void EngineLoader::LoadSystem(XMLElement* element, OmicronEngine* engine) {
        // TODO dynamically construct type
        std::string type = Attribute(element, "type");

        if(type == "MaterialSystem") {
            engine->AddSystem(new MaterialSystem);
            return;
        }

        if(type == "ScriptHost") {
            ScriptHost* scriptHost = new ScriptHost;
            scriptHost->InitLuaState();
            engine->AddSystem(scriptHost);
            return;
        }

        if(type == "PhysicsSystem") {
            XMLElement* propChild = element->FirstChildElement("Property");
            std::string propName = propChild->Attribute("name");
            glm::vec3 gravity(0.f, 9.81f, 0.f);
            if(propName == "Gravity") {
                propChild->QueryFloatAttribute("x", &gravity.x);
                propChild->QueryFloatAttribute("y", &gravity.y);
                propChild->QueryFloatAttribute("z", &gravity.z);
            }
            engine->AddSystem(new PhysicsSystem(gravity));
            return;
        }

        if(type == "PsychicInteractionSystem") {
            OmicronSystem* system = engine->GetSystem("Physics");
            if(!system) {
                printf("PsychicInteractionSystem cannot find dependency \"PhysicsSystem\"\n");
                return;
            }
            if(PhysicsSystem* phys = dynamic_cast<PhysicsSystem*>(system))
                engine->AddSystem(new PyschicInteractionSystem(phys));
            else printf("Unable to map discovered system to a physics system\n");
            return;
        }
    }

    void EngineLoader::LoadTemplate(XMLElement* element, OmicronEngine* engine) {
        OmicronEntity* entity = new OmicronEntity;

        std::string name = Attribute(element, "id");

        std::string tags = Attribute(element, "tags");
        if(!tags.empty()) {
            for(const std::string& tag : Utils::Split(tags, ';'))
                if(!tag.empty()) entity->AddTag(tag);
        }

        XMLElement* transformElement = element->FirstChildElement("Transform");
        if(transformElement != nullptr)
            LoadTransform(transformElement, entity);

        XMLElement* componentContainer = element->FirstChildElement("Components");

        if(componentContainer) {
            XMLElement* componentElement = componentContainer->FirstChildElement("Component");
            while(componentElement != nullptr) {
                LoadComponent(componentElement, entity, engine);
                componentElement = componentElement->NextSiblingElement("Component");
            }
        }
        engine->AddTemplate(name, entity);
    }

    void EngineLoader::LoadEntity(XMLElement* element, OmicronEngine* engine) {

        OmicronEntity* entity = nullptr;

        std::string temp = Attribute(element, "use");
        if(!temp.empty()) {
            entity = engine->UseTemplate(temp, false);
            if(!entity)
                printf("[WARNING] Unrecognised template requested, \"%s\"", temp);
        }

        if(!entity)
            entity = new OmicronEntity;

        std::string tags = Attribute(element, "tags");
        if(!tags.empty()) {
            for(const std::string& tag : Utils::Split(tags, ';'))
                if(!tag.empty()) entity->AddTag(tag);
        }

        XMLElement* transformElement = element->FirstChildElement("Transform");
        if(transformElement != nullptr)
            LoadTransform(transformElement, entity);

        XMLElement* componentContainer = element->FirstChildElement("Components");

        if(componentContainer) {
            XMLElement* componentElement = componentContainer->FirstChildElement("Component");
            while(componentElement != nullptr) {
                LoadComponent(componentElement, entity, engine);
                componentElement = componentElement->NextSiblingElement("Component");
            }
        }
        engine->AddEntity(entity);
    }

    void EngineLoader::LoadTransform(XMLElement* element, OmicronEntity* entity) {
        XMLElement* tra = element->FirstChildElement("Translation");
        XMLElement* rot = element->FirstChildElement("Rotation");
        XMLElement* scl = element->FirstChildElement("Scale");

        if(tra) {
            tra->QueryFloatAttribute("x", &entity->transform.Translation.x);
            tra->QueryFloatAttribute("y", &entity->transform.Translation.y);
            tra->QueryFloatAttribute("z", &entity->transform.Translation.z);
        }

        if(rot) {
            bool useEuler = false;
            rot->QueryBoolAttribute("euler", &useEuler);
            if(useEuler) {
                float roll = 0;
                float pitch = 0;
                float yaw = 0;

                rot->QueryFloatAttribute("roll", &roll);
                rot->QueryFloatAttribute("pitch", &pitch);
                rot->QueryFloatAttribute("yaw", &yaw);

                entity->transform.Rotation = glm::quat(glm::vec3(pitch, yaw, roll));

            }else{
                rot->QueryFloatAttribute("x", &entity->transform.Rotation.x);
                rot->QueryFloatAttribute("y", &entity->transform.Rotation.y);
                rot->QueryFloatAttribute("z", &entity->transform.Rotation.z);
                rot->QueryFloatAttribute("w", &entity->transform.Rotation.w);
            }
        }

        if(scl) {
            scl->QueryFloatAttribute("x", &entity->transform.Scale.x);
            scl->QueryFloatAttribute("y", &entity->transform.Scale.y);
            scl->QueryFloatAttribute("z", &entity->transform.Scale.z);
        }
    }

    void EngineLoader::LoadComponent(XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) {
        std::string type = Attribute(element, "type");

        if(type == "MeshComponent") {
            bool isPrimitive = false;
            std::string path = Attribute(element, "path");
            element->QueryBoolAttribute("primitive", &isPrimitive);

            std::map<std::string, int> params;

            XMLElement* paramsElement = element->FirstChildElement("Parameter");
            while(paramsElement) {
                std::string name = Attribute(paramsElement, "name");
                int value = 0;
                paramsElement->QueryIntAttribute("value", &value);
                params[name] = value;
                paramsElement = paramsElement->NextSiblingElement("Parameter");
            }

            if(isPrimitive)
                entity->SetComponent<MeshComponent>(MeshComponent::FromPrimitive(path, params));
            else entity->SetComponent<ModelComponent>(new ModelComponent(path));
            return;
        }

        if(type == "ScriptComponent") {
            XMLElement* scriptElement = element->FirstChildElement("Script");
            if(!scriptElement) {
                printf("[ERROR] No <Script> element found\n");
                return;
            }

            std::string path = Attribute(scriptElement, "path");
            std::string script;
            if(path.empty())
                script = scriptElement->GetText();
            else script = Files::ReadFile(path);
//            lua_State* L = nullptr;
//            if(path.empty())
//                L = scriptHost->ExecuteScript(scriptElement->GetText());
//            else L = scriptHost->LoadAndExecuteScript(path);

//            DEBUG_PRINT(luabridge::LuaRef exports = luabridge::getGlobal(L, "Exports"));
//            if(exports.isTable()) {
                // TODO support nested function structures
            XMLElement* bindings = element->FirstChildElement("Bindings");
            if(!bindings) {
                printf("[ERROR] No script bindings found\n");
                return;
            }
            XMLElement* binding = bindings->FirstChildElement("Binding");
            ScriptComponent* component = new ScriptComponent;
            component->script = script;
            std::string event;
            std::string func;
            while(binding) {
                event = binding->Attribute("event");
                func = binding->Attribute("function");
                component->scriptRefInfos.push_back(ScriptRefInfo(event, func));
                binding = binding->NextSiblingElement("Binding");
            }
            entity->SetComponent<ScriptComponent>(component);
            return;
        }

        if(type == "PhysicsComponent") {
            bool match = true;
            element->QueryBoolAttribute("match", &match);
            bool kinematic = false;
            element->QueryBoolAttribute("kinematic", &kinematic);
            float mass = 0.0f;
            element->QueryFloatAttribute("mass", &mass);
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
                }else{
                    printf("[ERROR] Unable to cast detected system to PhysicsSystem\n");
                    return;
                }
            }
        }

        if(type == "MaterialComponent") {
            std::string mtl = Attribute(element, "material");
            MaterialComponent* mtlComp = new MaterialComponent(mtl);
            float alpha = 1.f;
            element->QueryFloatAttribute("alpha", &alpha);
            mtlComp->alpha = alpha;
            XMLElement* uniformsContainer = element->FirstChildElement("Uniforms");
            if(uniformsContainer) {
                XMLElement* uniform = uniformsContainer->FirstChildElement("Uniform");
                while(uniform) {
                    std::string name = uniform->Attribute("name");
                    float value = -1;
                    uniform->QueryFloatAttribute("value", &value);
                    mtlComp->uniforms[name] = value;
                    uniform = uniform->NextSiblingElement("Uniform");
                }
            }
            entity->SetComponent<MaterialComponent>(mtlComp);
            return;
        }

    }

    std::string EngineLoader::Attribute(XMLElement* element, std::string name) {
        auto val = element->Attribute(name.c_str());
        std::string temp = "";
        if(val)
            temp = val;
        return temp;
    }

}
