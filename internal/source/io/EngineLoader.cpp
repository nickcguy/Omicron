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

    namespace {

        BaseRenderer* targetRenderer = nullptr;

        std::vector<IComponentFactory*>& GetFactories() {
            static std::vector<IComponentFactory*> factories;
            return factories;
        }
    }

    IComponentFactory* EngineLoader::GetComponentFactory(std::string name) {
        for(auto fac : GetFactories()) {
            if(fac->Name() == name)
                return fac;
        }
        return nullptr;
    }

    void EngineLoader::AddComponentFactory(IComponentFactory* fac) {
        std::string name = fac->Name();
        auto existing = GetComponentFactory(name);
        if(existing != nullptr)
            GetFactories().erase(std::remove(GetFactories().begin(), GetFactories().end(), existing), GetFactories().end());
        GetFactories().push_back(fac);
    }

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
        XMLElement* cubemapElement = element->FirstChildElement("Cubemap");
        if(cubemapElement) {
            std::string root = Attribute(cubemapElement, "root");
            std::vector<std::string> faces(6);

            XMLElement* panelElement = cubemapElement->FirstChildElement("Panel");
            int i = 0;
            while(panelElement && i < faces.size()) {
                std::string path = Attribute(panelElement, "path");
                faces[i++] = root + path;
                panelElement = panelElement->NextSiblingElement("Panel");
            }

            targetRenderer->SetCubemap(faces);

            XMLElement* dayNightElement = cubemapElement->FirstChildElement("DayNight");
            if(dayNightElement) {
                std::string path = Attribute(dayNightElement, "path");
                targetRenderer->SetGradient(root+path);
            }

        }
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
            glm::vec3 gravity(0.f, -9.81f, 0.f);

            while(propChild) {
                std::string propName = propChild->Attribute("name");
                if(propName == "Gravity") {
                    propChild->QueryFloatAttribute("x", &gravity.x);
                    propChild->QueryFloatAttribute("y", &gravity.y);
                    propChild->QueryFloatAttribute("z", &gravity.z);
                }
                propChild = propChild->NextSiblingElement("Property");
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

        auto fac = GetComponentFactory(type);
        if(fac) {
            fac->Load(element, entity, engine);
            return;
        }

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

    void EngineLoader::SetRenderer(BaseRenderer* target) {
        targetRenderer = target;
    }

}
