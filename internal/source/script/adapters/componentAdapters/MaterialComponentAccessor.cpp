//
// Created by Guy on 11/08/2017.
//

#include <script/adapters/componentAdapters/MaterialComponentAccessor.hpp>

namespace Omicron {

    MaterialComponentAccessor::MaterialComponentAccessor() = default;
    MaterialComponentAccessor::MaterialComponentAccessor(MaterialComponent* mtlComp) : mtlComp(mtlComp) {}

    std::string MaterialComponentAccessor::Get() {
        if(mtlComp)
            return mtlComp->materialId;
        return "";
    }

    void MaterialComponentAccessor::Set(std::string mtlId) {
        if(mtlComp)
            mtlComp->materialId = mtlId;
    }

    float MaterialComponentAccessor::GetUniform(std::string name) {
        if(mtlComp)
            return mtlComp->uniforms.find(name) != mtlComp->uniforms.end() ? mtlComp->uniforms[name] : 0.f;
        return 0.f;
    }

    void MaterialComponentAccessor::SetUniform(std::string name, float value) {
        if(mtlComp)
            mtlComp->uniforms[name] = value;
    }

    float MaterialComponentAccessor::GetAlpha() {
        if(mtlComp)
            return mtlComp->alpha;
        return 1.f;
    }

    void MaterialComponentAccessor::SetAlpha(float alpha) {
        if(mtlComp)
            mtlComp->alpha = alpha;
    }

    void MaterialComponentAdapter::Register(const sel::State& state) {
        state["MaterialComponent"].SetClass<MaterialComponent>(
            "Name", &MaterialComponent::Name,
            "GetUniform", &MaterialComponent::GetUniform,
            "SetUniform", &MaterialComponent::SetUniform,
            "HasUniform", &MaterialComponent::HasUniform,
            "GetAlpha", &MaterialComponent::GetAlpha,
            "SetAlpha", &MaterialComponent::SetAlpha
        );
    }

    const char* MaterialComponentAdapter::Name() {
        return "MaterialComponentAdapter";
    }
}