//
// Created by Guy on 19/08/2017.
//

#include <engine/EngineEventHandler.hpp>
#include <algorithm>

namespace Omicron {

    void EngineEventHandler::Update() {
        for(const auto& event : events)
            InvokeEvent(event);
        ClearEvents();
    }

    void EngineEventHandler::PostEvent(EngineEvent event) {
        events.push_back(event);
    }

    void EngineEventHandler::AddListener(std::string eventName, EngineEventListener* listener) {
        if(listeners.find(eventName) == listeners.end())
            listeners[eventName] = std::vector<EngineEventListener*>();
        listeners[eventName].push_back(listener);
    }

    void EngineEventHandler::RemoveListener(std::string eventName, EngineEventListener* listener) {
        if(listeners.find(eventName) != listeners.end()) {
            listeners[eventName].erase(std::remove(listeners[eventName].begin(), listeners[eventName].end(), listener), listeners[eventName].end());
            if(listeners[eventName].empty())
                listeners.erase(eventName);
        }
    }

    void EngineEventHandler::InvokeEvent(EngineEvent event) {
        for(auto listener : globalListeners)
            listener->Invoke(this, EventArgs{event.eventName, event.data});

        if(listeners.find(event.eventName) == listeners.end()) return;
        for(auto listener : listeners[event.eventName])
            listener->Invoke(this, EventArgs{event.eventName, event.data});
    }

    void EngineEventHandler::ClearEvents() {
        events.clear();
    }

    EngineEventHandler::EngineEventHandler(OmicronEngine* engine) : engine(engine) {}

    void EngineEventHandler::AddGlobalListener(EngineEventListener* listener) {
        globalListeners.push_back(listener);
    }

    void EngineEventHandler::RemoveGlobalListener(EngineEventListener* listener) {
        globalListeners.erase(std::remove(globalListeners.begin(), globalListeners.end(), listener), globalListeners.end());
    }

}