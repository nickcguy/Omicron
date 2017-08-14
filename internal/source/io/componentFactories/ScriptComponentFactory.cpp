//
// Created by Guy on 12/08/2017.
//

#include <io/componentFactories/ScriptComponentFactory.hpp>
#include <script/ScriptComponent.hpp>
#include <utils/FileUtils.hpp>
#include <io/EngineLoader.hpp>

namespace Omicron {

    void ScriptComponentFactory::Load(XMLElement* element, OmicronEntity* entity, OmicronEngine* engine) {
        XMLElement* scriptElement = element->FirstChildElement("Script");
        if(!scriptElement) {
            printf("[ERROR] No <Script> element found\n");
            return;
        }

        std::string path = EngineLoader::Attribute(scriptElement, "path");
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
    }

    std::string ScriptComponentFactory::Name() {
        return "ScriptComponent";
    }
}