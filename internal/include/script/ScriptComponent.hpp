//
// Created by Guy on 10/08/2017.
//

#ifndef OMICRONRENDER_SCRIPTCOMPONENT_HPP
#define OMICRONRENDER_SCRIPTCOMPONENT_HPP

#include <engine/component/OmicronComponent.hpp>
#include <memory>
#include <map>
#include <utility>
#include <script/adapters/OmicronEntityAccessor.hpp>
#include <script/adapters/OmicronEngineAccessor.hpp>
#include <engine/OmicronEngine.hpp>

namespace Omicron {

    struct ScriptRefInfo {
        ScriptRefInfo(std::string bind, std::string func) : bindName(std::move(bind)), funcName(std::move(func)){}

        std::string bindName;
        std::string funcName;
    };

    struct ScriptRef {
        std::string bindName;
        std::string funcName;
        sel::Selector funcRef;

        ScriptRef(std::string bindName, std::string funcName, const sel::Selector& func)
        : bindName(std::move(bindName)), funcName(std::move(funcName)), funcRef(func) {}

        ScriptRef(const ScriptRef& other) : bindName(other.bindName), funcName(other.funcName), funcRef(other.funcRef) {}

        explicit ScriptRef(const ScriptRef* other) : bindName(other->bindName), funcName(other->funcName), funcRef(other->funcRef) {}

        inline void Invoke(float delta, OmicronEngine* world, OmicronEntity* entity) {
            try {
                funcRef(delta, world, entity);
            }catch(...) {
                printf("Uncaught exception\n");
            }
        }

        inline void Invoke(float delta, OmicronEngine* world, OmicronEntity* entity, PrimitiveVariant arg) {
            switch(arg.type) {
                case PRIMITIVE_NONE:  Invoke(delta, world, entity); break;
                case PRIMITIVE_INT:   Invoke(delta, world, entity, arg.integer); break;
                case PRIMITIVE_BOOL:  Invoke(delta, world, entity, arg.boolean); break;
                case PRIMITIVE_FLOAT: Invoke(delta, world, entity, arg.floating); break;
            }
        }

        template <typename T>
        inline void Invoke(float delta, OmicronEngine* world, OmicronEntity* entity, T argV) {
            try {
                funcRef(delta, world, entity, argV);
            }catch(...) {
                printf("Uncaught exception\n");
            }
        }
    };

    class ScriptComponent : public OmicronComponent {
    public:

        ScriptComponent() = default;
        inline explicit ScriptComponent(ScriptComponent* other) {
            scriptRefs = other->scriptRefs;
        }

        inline virtual OmicronComponent* Clone() override {
            return new ScriptComponent(this);
        }

        inline ScriptRef* GetScriptRef(std::string name) {
            return scriptRefs.find(name) != scriptRefs.end() ? scriptRefs[name] : nullptr;
        }

        inline void RemoveScriptRef(ScriptRef* ref) {
            RemoveScriptRef(ref->bindName);
        }
        inline void RemoveScriptRef(ScriptRef ref) {
            RemoveScriptRef(ref.bindName);
        }
        inline void RemoveScriptRef(std::string name) {
            scriptRefs.erase(name);
        }

        virtual std::string Name() override {
            return "ScriptComponent";
        }

        bool hasExecuted = false;
        std::string script;
        std::map<std::string, ScriptRef*> scriptRefs;
        std::vector<ScriptRefInfo> scriptRefInfos;

        sel::State* state = nullptr;

    };

}

#endif //OMICRONRENDER_SCRIPTCOMPONENT_HPP
