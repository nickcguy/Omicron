//
// Created by Guy on 09/08/2017.
//

#include <script/adapters/OmicronMaterialAdapter.hpp>
#include <data/material/OmicronMaterial.hpp>
#include <data/material/MaterialManager.hpp>

namespace Omicron {

    MaterialAccessor::MaterialAccessor(const std::string& name) : name(name) {}


    float MaterialAccessor::GetUniform(std::string name) {
        return GetMaterial()->GetUniform(name);
    }

    void MaterialAccessor::SetUniform(std::string name, float value) {
        GetMaterial()->SetUniform(name, value);
    }

    void MaterialAccessor::AddInterpValue(std::string name, float target, float duration) {
        GetMaterial()->AddInterpolatingValue(name, target, duration);
    }

    OmicronMaterial* MaterialAccessor::GetMaterial() {
        return MaterialManager::GetActiveMaterialManager()->GetMaterial(this->name, false);
    }

    void OmicronMaterialAdapter::Register(const sel::State& state) {

    }

    const char* OmicronMaterialAdapter::Name() {
        return "OmicronMaterialAdapter";
    }

}