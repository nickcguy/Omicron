//
// Created by Guy on 09/08/2017.
//

#include <pch.hpp>
#include <lua.hpp>
#include <script/IScriptable.hpp>
#include <script/ScriptHost.hpp>
#include <utils/FileUtils.hpp>
#include <script/adapters/OmicronMaterialAdapter.hpp>


#include <iostream>
#include <script/ScriptComponent.hpp>
#include <engine/OmicronEngine.hpp>
#include <script/adapters/SystemAdapter.hpp>
#include <script/adapters/OmicronEngineAccessor.hpp>
#include <script/adapters/componentAdapters/MaterialComponentAccessor.hpp>

#if USE_LUABRIDGE
#include <LuaBridge/LuaBridge.h>
using namespace luabridge;
#else

#endif

#include <script/IScriptable.hpp>

namespace Omicron {

    namespace {
        std::vector<IScriptable*>& GetScriptables() {
            static std::vector<IScriptable*> scriptables;
            return scriptables;
        }
    }

    void ScriptHost::RegisterScriptableAdapter(IScriptable* scriptable) {
        std::string name(scriptable->Name());
        for(auto s : GetScriptables()) {
            if(name == s->Name()) {
                CLEAR_PTR(scriptable)
                return;
            }
        }
        GetScriptables().push_back(scriptable);
    }

    void ScriptHost::InitLuaState() {
        printf("Initializing %s, %s\n", LUA_RELEASE, LUA_COPYRIGHT);

        #if USE_LUABRIDGE
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);

        auto ns = luabridge::getGlobalNamespace(L);

        try {

            ns.beginNamespace("Omicron")
              .beginClass<SystemAdapter>("System")
                .addConstructor <void (*)(void)>()
                .addFunction("PrintMessage", &SystemAdapter::PrintMessage)
              .endClass()
              .beginClass<OmicronEngineAccessor>("Engine")
                .addConstructor<void (*)(std::string)>()
                .addFunction("GetTaggedEntityCount", &OmicronEngineAccessor::GetTaggedCount)
                .addFunction("GetTaggedEntity", &OmicronEngineAccessor::GetTaggedEntity)
                .addFunction("GetEntityCount", &OmicronEngineAccessor::GetEntityCount)
              .endClass()
              .beginClass<MaterialAccessor>("Material")
                .addConstructor<void (*)(std::string)>()
                .addFunction("GetUniform", &MaterialAccessor::GetUniform)
                .addFunction("SetUniform", &MaterialAccessor::SetUniform)
                .addFunction("Interp", &MaterialAccessor::AddInterpValue)
              .endClass()
              .beginClass<OmicronEntityAccessor>("Entity")
                .addConstructor<void (*)(void)>()
                .addFunction("GetTags", &OmicronEntityAccessor::GetTags)
                .addFunction("GetTagString", &OmicronEntityAccessor::GetTagString)
              .endClass()
            .endNamespace();

        }catch (LuaException& exc) {
            printf("LuaException: %s\n", exc.what());
        }

        LSet.push_back(L);

        #else

        RegisterScriptableAdapters();
//        for(auto s : scriptables)
//            s->Register(state);

        #endif


//        auto omicronNs = ns.beginNamespace("Omicron");
//        ns.addFunction("printMessage", PrintMessage);

//        omicronNs.endNamespace();
    }

    sel::State* ScriptHost::LoadAndExecuteScript(std::string fileName) {
        std::string contents = Files::ReadFile(fileName);
        return ExecuteScript(contents);
    }

    sel::State* ScriptHost::ExecuteScript(std::string script) {
        printf("Executing script: [%s]\n", script.c_str());

        sel::State* state = new sel::State{true};

        for(auto s : GetScriptables())
            s->Register(*state);

        state->LoadRaw(script);
        return state;
    }

    void ScriptHost::Destroy() {
//        scriptables.clear();
    }

    void ScriptHost::RegisterScriptableAdapters() {
//        RegisterScriptableAdapter(new SystemAdapter);
//        RegisterScriptableAdapter(new OmicronEntityAdapter);
//        RegisterScriptableAdapter(new OmicronEngineAdapter);
//        RegisterScriptableAdapter(new EntityComponentAdapter);
//        RegisterScriptableAdapter(new MaterialComponentAdapter);
    }

    int ScriptHost::PCall(int argCount, int retCount) {
        // Push msgh

        static char key;

//        lua_pushlightuserdata(L, &key);
//        lua_rawget(L, LUA_REGISTRYINDEX);
//        int msgh = -(argCount+2);
//        lua_insert(L, msgh);
//
//        int result = lua_pcall(L, argCount, retCount, msgh);
//
//        // Remove msgh
//        if(result == 0)
//            lua_remove(L, -(retCount+1));
//        else
//            lua_remove(L, -2);

        return 0;
    }

    void ScriptHost::Update(float delta) {
        std::vector<OmicronEntity*> entities = engine->GetEntitiesWith<ScriptComponent>();
        std::vector<ScriptEvent> events = GetEvents();
        static const std::string tickEventName = "Tick";
        events.push_back(ScriptEvent{tickEventName});

        auto engineAdapter = OmicronEngineAccessor("CoreEngine");

        for(auto entity : entities) {
            ScriptComponent* comp = entity->GetCastComponent<ScriptComponent>();
            if(comp->scriptRefInfos.empty()) continue;
            std::string script = comp->script;
            comp->state = ExecuteScript(script);

            sel::Selector exports = (*comp->state)["Exports"];

            if(!exports.exists()) {
                printf("[ERROR] No exports table found\n");
                continue;
            }
            for(auto info : comp->scriptRefInfos) {
                sel::Selector ref = exports[info.funcName];
                if(ref.exists()) {
                    ScriptRef* sRef = new ScriptRef(info.bindName, info.funcName, ref);
                    comp->scriptRefs[info.bindName] = sRef;
                }
            }
            comp->scriptRefInfos.clear();

            if(!comp->hasExecuted) {
                comp->hasExecuted = true;
                ScriptRef* ref = comp->GetScriptRef("BeginPlay");
                auto entityAccessor = OmicronEntityAccessor(entity);
                ref->Invoke(delta, engineAdapter, entityAccessor, 0, nullptr);
            }
        }


        if(processEventsPerEntity) {
            for(auto entity : entities) {
                ScriptComponent* comp = entity->GetCastComponent<ScriptComponent>();

                auto entityAccessor = OmicronEntityAccessor(entity);

//                (*comp->state)["Delta"]  = delta;
//                (*comp->state)["World"]  = engineAdapter;
//                (*comp->state)["Entity"] = entityAccessor;

                for(auto event : events) {
                    ScriptRef* ref = comp->GetScriptRef(event.eventName);
                    if(!ref) continue;
                    ref->Invoke(delta, engineAdapter, entityAccessor, event.argCount, event.args);
                    if(event.eventName != tickEventName)
                        comp->RemoveScriptRef(ref);
                }
            }
        }else{
            for(auto event : events) {
                for(auto entity : entities) {
                    ScriptComponent* comp = entity->GetCastComponent<ScriptComponent>();
                    ScriptRef* ref = comp->GetScriptRef(event.eventName);
                    if(!ref) continue;

//                    (*comp->state)["Imports"]["Delta"]  = delta;
//                    (*comp->state)["Imports"]["World"]  = engineAdapter;
//                    (*comp->state)["Imports"]["Entity"] = OmicronEntityAccessor(entity);

                    auto ent = OmicronEntityAccessor(entity);

                    ref->Invoke(delta, engineAdapter, ent, event.argCount, event.args);
                    if(event.eventName != tickEventName)
                        comp->RemoveScriptRef(ref);
                }
            }
        }

    }

    std::string ScriptHost::Name() {
        return "Script";
    }

    std::vector<ScriptEvent> ScriptHost::GetEvents() {
        std::lock_guard<std::mutex> guard(eventMutex);
        std::vector<ScriptEvent> snapshot(events);
        events.clear();
        return snapshot;
    }

    void ScriptHost::PostEvent(ScriptEvent event) {
        std::lock_guard<std::mutex> guard(eventMutex);
        events.push_back(event);
    }

}