//
// Created by Guy on 11/08/2017.
//

#include <script/adapters/componentAdapters/EntityComponentAccessor.hpp>
#include <script/adapters/OmicronEntityAccessor.hpp>

namespace Omicron {


    EntityComponentAccessor::EntityComponentAccessor() {}
    EntityComponentAccessor::EntityComponentAccessor(OmicronEntityAccessor* entity) : entity(entity->GetEntity()) {}

    MaterialComponentAccessor* EntityComponentAccessor::GetMaterial() {
        if(entity)
            if(entity->HasComponent<MaterialComponent>())
                return new MaterialComponentAccessor(entity->GetCastComponent<MaterialComponent>());
        return nullptr;
    }

    PhysicsComponentAccessor* EntityComponentAccessor::GetPhysics() {
        if(entity)
            if(entity->HasComponent<PhysicsComponent>())
                return new PhysicsComponentAccessor(entity->GetCastComponent<PhysicsComponent>());
        return nullptr;
    }

    void EntityComponentAdapter::Register(const sel::State& state) {
        state["ComponentAccessor"].SetClass<EntityComponentAccessor, OmicronEntityAccessor*>(
            "GetMaterial", &EntityComponentAccessor::GetMaterial,
            "GetPhysics", &EntityComponentAccessor::GetPhysics
        );
    }

    const char* EntityComponentAdapter::Name() {
        return "EntityComponentAdapter";
    }

    /*
     * MaterialComponentAccessor* OmicronEntityAccessor::GetMaterialComponent() {
        if(entity->HasComponent<MaterialComponent>())
            return new MaterialComponentAccessor(entity->GetCastComponent<MaterialComponent>());
    }

    PhysicsComponentAccessor* OmicronEntityAccessor::GetPhysicsComponent() {
        if(entity->HasComponent<PhysicsComponent>())
            return new PhysicsComponentAccessor(entity->GetCastComponent<PhysicsComponent>());
        return nullptr;
    }
     */

}