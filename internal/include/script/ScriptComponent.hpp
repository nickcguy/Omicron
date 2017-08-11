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

        inline void Invoke(float delta, OmicronEngineAccessor world, OmicronEntityAccessor entity, int argC, void** argV) {
                switch(argC) {
                    case 0: funcRef(delta, world, entity); break;
                    case 1: funcRef(delta, world, entity, argV[0]); break;
                    case 2: funcRef(delta, world, entity, argV[0], argV[1]); break;
                    case 3: funcRef(delta, world, entity, argV[0], argV[1], argV[2]); break;
                    case 4: funcRef(delta, world, entity, argV[0], argV[1], argV[2], argV[3]); break;
                    case 5: funcRef(delta, world, entity, argV[0], argV[1], argV[2], argV[3], argV[4]); break;
//                    case 6: funcRef(argV[0], argV[1], argV[2], argV[3], argV[4], argV[5]); break;
//                    case 7: funcRef(argV[0], argV[1], argV[2], argV[3], argV[4], argV[5], argV[6]); break;
//                    case 8: funcRef(argV[0], argV[1], argV[2], argV[3], argV[4], argV[5], argV[6], argV[7]); break;
                    default:
                        printf("[ERROR] Too many arguments, %i is more than 6\n", argC);
                        break;
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

        sel::State* state;

    };

}

#endif //OMICRONRENDER_SCRIPTCOMPONENT_HPP
