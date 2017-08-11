//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_OMICRONMATERIALADAPTER_HPP
#define OMICRONRENDER_OMICRONMATERIALADAPTER_HPP

#include <script/IScriptable.hpp>
#include <data/material/OmicronMaterial.hpp>

namespace Omicron {

    struct MaterialAccessor {
        std::string name;
        int instance;

        MaterialAccessor(const std::string& name);

        OmicronMaterial* GetMaterial();

        float GetUniform(std::string name);
        void SetUniform(std::string name, float value);
        void AddInterpValue(std::string name, float target, float duration);
    };

    class OmicronMaterialAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;
        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(OmicronMaterialAdapter)

}

#endif //OMICRONRENDER_OMICRONMATERIALADAPTER_HPP
