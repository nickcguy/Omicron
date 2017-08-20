//
// Created by Guy on 11/08/2017.
//

#include <script/adapters/componentAdapters/EntityComponentAccessor.hpp>
#include <script/adapters/OmicronEntityAccessor.hpp>

namespace Omicron {


    EntityComponentAccessor::EntityComponentAccessor() {}

    MaterialComponent* EntityComponentAccessor::GetMaterial(OmicronEntity* entity) {
        if(entity)
            if(entity->HasComponent<MaterialComponent>())
                return entity->GetCastComponent<MaterialComponent>();
        return nullptr;
    }

    PhysicsComponent* EntityComponentAccessor::GetPhysics(OmicronEntity* entity) {
        if(entity)
            if(entity->HasComponent<PhysicsComponent>())
                return entity->GetCastComponent<PhysicsComponent>();
        return nullptr;
    }

    Light* EntityComponentAccessor::GetLight(OmicronEntity* entity) {
        if(entity)
            if(entity->HasComponent<LightComponent>())
                return entity->GetCastComponent<LightComponent>()->lightData;
        return nullptr;
    }

    void EntityComponentAdapter::Register(const sel::State& state) {
        state["ComponentAccessor"].SetClass<EntityComponentAccessor>(
            "GetMaterial", &EntityComponentAccessor::GetMaterial,
            "GetPhysics", &EntityComponentAccessor::GetPhysics,
            "GetLight", &EntityComponentAccessor::GetLight
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