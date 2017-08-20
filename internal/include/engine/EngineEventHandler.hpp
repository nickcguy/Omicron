//
// Created by Guy on 19/08/2017.
//

#ifndef OMICRONRENDER_ENGINEEVENTHANDLER_HPP
#define OMICRONRENDER_ENGINEEVENTHANDLER_HPP

#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include "EventArgs.hpp"

namespace Omicron {

    class OmicronEngine;
    class EngineEventHandler;

    class EngineEventListener {
    public:
        virtual void Invoke(EngineEventHandler* handler, EventArgs args)=0;
    };

    class LambdaEngineEventListener : public EngineEventListener {
    public:
        LambdaEngineEventListener(std::function<void(EngineEventHandler*, EventArgs)> func) : func(std::move(func)) {}

        inline void Invoke(EngineEventHandler* handler, EventArgs args) override {
            func(handler, args);
        }
    protected:
        std::function<void(EngineEventHandler* handler, EventArgs)> func;
    };

    class EngineEventHandler {
    public:
        EngineEventHandler(OmicronEngine* engine);

        void Update();
        void PostEvent(EngineEvent event);

        void AddListener(std::string eventName, EngineEventListener* listener);
        void RemoveListener(std::string eventName, EngineEventListener* listener);

        void AddGlobalListener(EngineEventListener* listener);
        void RemoveGlobalListener(EngineEventListener* listener);

    protected:

        void InvokeEvent(EngineEvent event);
        void ClearEvents();

        std::vector<EngineEvent> events;
        std::map<std::string, std::vector<EngineEventListener*>> listeners;
        OmicronEngine* engine;
        std::vector<EngineEventListener*> globalListeners;
    };

}

#endif //OMICRONRENDER_ENGINEEVENTHANDLER_HPP
