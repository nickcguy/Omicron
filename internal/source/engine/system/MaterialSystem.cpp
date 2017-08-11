//
// Created by Guy on 09/08/2017.
//

#include <engine/system/MaterialSystem.hpp>
#include <engine/component/MaterialComponent.hpp>
#include <engine/OmicronEngine.hpp>

namespace Omicron {

    void MaterialSystem::Update(float delta) {
        FilteredVector<OmicronEntity*> entities = engine->GetEntitiesWith<MaterialComponent>();
        if(entities.empty()) return;
//        for(OmicronEntity* entity : entities) {
//            MaterialComponent* mtlComp = entity->GetCastComponent<MaterialComponent>();
//            mtlComp->MakeDynamic(MaterialManager::GetActiveMaterialManager());
//            OmicronMaterial* mtl = MaterialManager::GetActiveMaterialManager()->GetMaterial(mtlComp->materialId, mtlComp->instanceId, true);
//            if(mtl) {
//                for(auto pair : mtlComp->uniforms)
//                    mtl->SetUniform(pair.first, pair.second);
//            }
//        }
    }

    std::string MaterialSystem::Name() {
        return "Material";
    }
}