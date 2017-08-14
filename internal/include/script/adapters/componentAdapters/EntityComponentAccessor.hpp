//
// Created by Guy on 11/08/2017.
//

#ifndef OMICRONRENDER_ENTITYCOMPONENTACCESSOR_HPP
#define OMICRONRENDER_ENTITYCOMPONENTACCESSOR_HPP

#include "MaterialComponentAccessor.hpp"
#include "PhysicsComponentAccessor.hpp"
#include <string>
#include <script/IScriptable.hpp>
#include <script/ScriptHost.hpp>

namespace Omicron {

    class OmicronEntityAccessor;

    class EntityComponentAccessor {
    public:
        EntityComponentAccessor();
        EntityComponentAccessor(OmicronEntityAccessor* entity);

        MaterialComponentAccessor* GetMaterial();
        PhysicsComponentAccessor* GetPhysics();

    protected:
        OmicronEntity* entity = nullptr;
    };

    class EntityComponentAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;
        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(EntityComponentAdapter)

}

#endif //OMICRONRENDER_ENTITYCOMPONENTACCESSOR_HPP
