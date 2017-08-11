//
// Created by Guy on 10/08/2017.
//

#include <script/adapters/OmicronEngineAccessor.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/OmicronEngineWrapper.hpp>
#include <utility>

namespace Omicron {

//    OmicronEngineAccessor::OmicronEngineAccessor() {}

    OmicronEngineAccessor::OmicronEngineAccessor(std::string engineName) : engineName(std::move(engineName)) {}

//    OmicronEngineAccessor::OmicronEngineAccessor(OmicronEngine* engine) : engine(engine) {}

    std::vector<OmicronEntityAccessor> OmicronEngineAccessor::GetTagged(std::string tag) {
        std::vector<OmicronEntityAccessor> entityAdapters;
        for(auto entity : GetEngine()->GetTaggedEntities(tag))
            entityAdapters.emplace_back(entity);
        return entityAdapters;
    }

    int OmicronEngineAccessor::GetEntityCount() {
        return GetEngine()->EntityCount();
    }

    OmicronEngine* OmicronEngineAccessor::GetEngine() {
        return OmicronEngineWrapper::GetEngineWrapper("CoreEngine")->GetChild();
//        return OmicronEngineWrapper::GetEngineWrapper(engineName)->GetChild();
    }

    int OmicronEngineAccessor::GetTaggedCount(std::string tag) {
        return GetTagged(tag).size();
    }

    OmicronEntityAccessor OmicronEngineAccessor::GetTaggedEntity(std::string tag, int id) {
        std::vector<OmicronEntityAccessor> tagged = GetTagged(tag);
        return tagged[id];
    }

    void OmicronEngineAdapter::Register(const sel::State& state) {
        state["Engine"].SetClass<OmicronEngineAccessor, std::string>(
            "GetEntityCount", &OmicronEngineAccessor::GetEntityCount,
            "GetTagged", &OmicronEngineAccessor::GetTagged,
            "GetTaggedCount", &OmicronEngineAccessor::GetTaggedCount,
            "GetTaggedEntity", &OmicronEngineAccessor::GetTaggedEntity
        );
    }

    const char* OmicronEngineAdapter::Name() {
        return "OmicronEngineAdapter";
    }
}