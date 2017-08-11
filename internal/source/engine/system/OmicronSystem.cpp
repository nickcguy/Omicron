//
// Created by Guy on 08/08/2017.
//

#include <engine/system/OmicronSystem.hpp>

namespace Omicron {

    OmicronSystem::OmicronSystem() = default;
    void OmicronSystem::SetEngine(OmicronEngine* engine) {
        this->engine = engine;
    }

    bool OmicronSystem::IsActive() const {
        return active && engine != nullptr;
    }

    void OmicronSystem::SetActive(bool active) {
        OmicronSystem::active = active;
    }

    void OmicronSystem::Destroy() {

    }

}