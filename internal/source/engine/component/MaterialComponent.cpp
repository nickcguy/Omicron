//
// Created by Guy on 07/08/2017.
//

#include <engine/component/MaterialComponent.hpp>

namespace Omicron {

    MaterialComponent::MaterialComponent(const std::string& materialId) : materialId(materialId) {}


    MaterialComponent::~MaterialComponent() {
    }

    OmicronComponent* MaterialComponent::Clone() {
        return new MaterialComponent(this);
    }

    MaterialComponent::MaterialComponent(MaterialComponent* other) {
        materialId = other->materialId;
        alpha = other->alpha;
        uniforms = other->uniforms;
    }

    std::string MaterialComponent::Name() {
        return "Material";
    }

    MaterialComponent::MaterialComponent() = default;

}