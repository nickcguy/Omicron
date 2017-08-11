//
// Created by Guy on 06/08/2017.
//

#include <engine/OmicronEngineWrapper.hpp>
#include <engine/OmicronEngine.hpp>

namespace Omicron {

    namespace {
        std::map<std::string, OmicronEngineWrapper*> engineWrapperCache;
    }

    OmicronEngineWrapper::OmicronEngineWrapper(std::string name, int targetIps) : name(name), engineThread(&OmicronEngine::Init, new OmicronEngine(targetIps, this)) {
        engineWrapperCache[name] = this;
    }

    void OmicronEngineWrapper::Stop() {
        child->Kill();
        if(engineThread.joinable())
            engineThread.join();

        if(child != nullptr) {
            delete child;
            child = nullptr;
        }
    }

    void OmicronEngineWrapper::SetChild(OmicronEngine* child) {
        this->child = child;
    }

    OmicronEngine* OmicronEngineWrapper::GetChild() {
        return this->child;
    }

    void OmicronEngineWrapper::SetTargetIPS(int targetIps) {
        if(this->child)
            this->child->SetTargetIPS(targetIps);
    }


    OmicronEngineWrapper* OmicronEngineWrapper::GetEngineWrapper(std::string name) {
        if(engineWrapperCache.find(name) != engineWrapperCache.end())
            return engineWrapperCache[name];
        return nullptr;
    }

}