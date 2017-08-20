//
// Created by Guy on 19/08/2017.
//

#include <engine/component/LightComponent.hpp>

namespace Omicron {


    void LightComponentAdapter::Register(const sel::State& state) {
        state["LightComponent"].SetClass<LightComponent>(
            "GetLight", &LightComponent::GetLightData
        );
    }

    const char* LightComponentAdapter::Name() {
        return "LightComponentAdapter";
    }

    void LightAdapter::Register(const sel::State& state) {
        state["Light"].SetClass<Light>(
//            "position", &Light::position,
//            "direction", &Light::direction,
//            "colour", &Light::colour,
            "cutoff", &Light::cutoff,
            "outerCutoff", &Light::outerCutoff,
            "constant", &Light::constant,
            "linear", &Light::linear,
            "quadratic", &Light::quadratic,
            "intensity", &Light::intensity,
//            "active", &Light::active,
            "Toggle", &Light::ToggleActive
        );
    }

    const char* LightAdapter::Name() {
        return "LightAdapter";
    }


}