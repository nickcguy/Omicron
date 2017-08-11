//
// Created by Guy on 10/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENTITYADAPTER_HPP
#define OMICRONRENDER_OMICRONENTITYADAPTER_HPP

#include <script/IScriptable.hpp>
#include <engine/entity/OmicronEntity.hpp>
#include <script/adapters/componentAdapters/MaterialComponentAccessor.hpp>
#include <script/adapters/componentAdapters/PhysicsComponentAccessor.hpp>

namespace Omicron {

    class OmicronEntityAccessor {
    public:
        OmicronEntityAccessor();

        OmicronEntityAccessor(OmicronEntity* entity);

        std::string* GetTags();
        std::string GetTagString();

        OmicronEntity* GetEntity();

    protected:
        OmicronEntity* entity;
    };

    class OmicronEntityAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(OmicronEntityAdapter)

}

#endif //OMICRONRENDER_OMICRONENTITYADAPTER_HPP
