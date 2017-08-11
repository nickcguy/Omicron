//
// Created by Guy on 11/08/2017.
//

#ifndef OMICRONRENDER_MATERIALCOMPONENTACCESSOR_HPP
#define OMICRONRENDER_MATERIALCOMPONENTACCESSOR_HPP

#include <engine/component/MaterialComponent.hpp>
#include <script/IScriptable.hpp>

namespace Omicron {

    class MaterialComponentAccessor {
    public:
        MaterialComponentAccessor();
        explicit MaterialComponentAccessor(MaterialComponent* mtlComp);

        std::string Get();
        void Set(std::string mtlId);

        float GetUniform(std::string name);
        void SetUniform(std::string name, float value);

        float GetAlpha();
        void SetAlpha(float alpha);

    protected:
        MaterialComponent* mtlComp = nullptr;

    };

    class MaterialComponentAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(MaterialComponentAdapter)

}

#endif //OMICRONRENDER_MATERIALCOMPONENTACCESSOR_HPP
