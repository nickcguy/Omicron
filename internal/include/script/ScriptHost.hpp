//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_SCRIPTHOST_HPP
#define OMICRONRENDER_SCRIPTHOST_HPP

#include <lua.hpp>

#include <string>
#include <vector>
#include <engine/system/OmicronSystem.hpp>
#include <mutex>
#include <selene.h>

namespace Omicron {

    struct ScriptEvent {
        std::string eventName;

        int argCount = 0;
        void* args[7];
    };

    class IScriptable;

    class ScriptHost : public OmicronSystem {
    public:

        inline ScriptHost() {}

        static void RegisterScriptableAdapter(IScriptable* scriptable);
        void RegisterScriptableAdapters();
        void InitLuaState();


        sel::State* LoadAndExecuteScript(std::string fileName);
        sel::State* ExecuteScript(std::string script);

        virtual void Destroy() override;

        int PCall(int argCount, int retCount);

        virtual void Update(float delta) override;

        virtual std::string Name() override;

        void PostEvent(ScriptEvent event);
        std::vector<ScriptEvent> GetEvents();

    protected:

        /**
         * @True Processes the events bound to an entity before moving on to the next entity
         * @False Processes the entities listening to an event before moving on to the next event
         */
        bool processEventsPerEntity = true;
        bool FirstRun = true;

        std::mutex eventMutex;
        sel::State state{true};
//        std::vector<sel::State> LSet;
        std::vector<ScriptEvent> events;
    };

}

#endif //OMICRONRENDER_SCRIPTHOST_HPP
