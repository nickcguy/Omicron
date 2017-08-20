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

    bool MaterialComponent::HasUniform(std::string name) {
        return uniforms.find(name) != uniforms.end();
    }

    float MaterialComponent::GetUniform(std::string name) {
        if(HasUniform(name))
            return uniforms[name];
        return -1;
    }

    float MaterialComponent::GetAlpha() {
        return alpha;
    }

    void MaterialComponent::SetUniform(std::string name, float value) {
        uniforms[name] = value;
    }

    void MaterialComponent::SetAlpha(float alpha) {
        this->alpha = alpha;
    }

}