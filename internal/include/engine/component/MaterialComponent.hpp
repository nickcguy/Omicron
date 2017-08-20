//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_MATERIALCOMPONENT_HPP
#define OMICRONRENDER_MATERIALCOMPONENT_HPP

#include "OmicronComponent.hpp"
#include <string>
#include <data/material/MaterialManager.hpp>

namespace Omicron {

    class MaterialComponent : public OmicronComponent {
    public:
        MaterialComponent(const std::string& materialId);
        MaterialComponent(MaterialComponent* other);
        MaterialComponent();
        ~MaterialComponent();

        virtual OmicronComponent* Clone() override;

        virtual std::string Name() override;

        std::string materialId = "";
        float alpha = 1.f;

        std::map<std::string, float> uniforms;

        bool HasUniform(std::string name);

        float GetUniform(std::string name);
        float GetAlpha();
        void SetUniform(std::string name, float value);
        void SetAlpha(float alpha);

    };

}

#endif //OMICRONRENDER_MATERIALCOMPONENT_HPP
