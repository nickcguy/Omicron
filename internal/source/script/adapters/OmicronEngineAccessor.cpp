//
// Created by Guy on 10/08/2017.
//

#include <script/adapters/OmicronEngineAccessor.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/OmicronEngineWrapper.hpp>
#include <utility>

namespace Omicron {

    OmicronEngineAccessor::OmicronEngineAccessor() {}

    InputState* OmicronEngineAccessor::GetInputState(OmicronEngine* engine) {
        auto inputs = engine->GetSystem("InputSystem");
        if(inputs)
            return dynamic_cast<InputSystem*>(inputs)->GetInputState();
        return new InputState;
    }

    void OmicronEngineAdapter::Register(const sel::State& state) {
        state["Engine"].SetClass<OmicronEngine>(
            "GetEntities", &OmicronEngine::GetEntities,
            "GetEntityCount", &OmicronEngine::EntityCount,
            "GetTagged", &OmicronEngine::GetTaggedEntities,
            "GetTaggedCount", &OmicronEngine::GetTaggedCount,
            "GetTaggedEntities", &OmicronEngine::GetTaggedEntities,
            "GetTaggedEntitiesAll", &OmicronEngine::GetTaggedEntities_All
        );

        state["EngineAccessor"].SetClass<OmicronEngineAccessor>(
            "GetInput", &OmicronEngineAccessor::GetInputState
        );
    }

    const char* OmicronEngineAdapter::Name() {
        return "OmicronEngineAdapter";
    }
}