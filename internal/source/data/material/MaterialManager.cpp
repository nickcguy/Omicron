//
// Created by Guy on 07/08/2017.
//

#include <data/material/MaterialManager.hpp>
#include <utils/FileUtils.hpp>
#include <data/SSBO.hpp>


namespace Omicron {

    namespace {
        MaterialManager* ActiveMaterialManager = nullptr;
    }

    OmicronMaterial* MaterialManager::GetMaterialBase(std::string name, bool validOnly) {
        if(HasMaterial(name)) {
            auto mtl = materialRegistry[name];
            if(validOnly) {
                if(mtl->IsReady())
                    return mtl;
            }else{
                return mtl;
            }
        }
        return defaultMaterial;
    }

    void MaterialManager::RegisterMaterial(OmicronMaterial* mtl) {
        materialRegistry[mtl->GetName()] = mtl;
    }

    bool MaterialManager::HasMaterial(std::string name) {
        return materialRegistry.find(name) != materialRegistry.end();
    }

    void MaterialManager::SetDefaultMaterial(OmicronMaterial* mtl) {
        defaultMaterial = mtl;
    }

    void MaterialManager::RegisterMaterials() {
        std::string mtlPath = "assets/materials";
        std::vector<std::string> files = Files::GetFilesInDirectory(mtlPath, ".mtl");
        for(std::string file : files) {
            printf("Material file found at \"%s\"\n", file.c_str());
            OmicronMaterial* mtl = new OmicronMaterial;
            std::string name = mtl->LoadNameFromFile(mtlPath + "/" + file);
            threadPool.push([this, mtl](int id) {
                mtl->LoadFromMemory();
                mtl->SetReady(true);
                QueueMaterial(mtl);
            });
        }
    }

    void MaterialManager::QueueMaterial(OmicronMaterial* mtl) {
        std::lock_guard<std::mutex> guard(materialQueueMutex);
        materialQueue.push(mtl);
        RegisterMaterial(mtl);
    }

    void MaterialManager::LinkUBO(UBO& ubo) {
        ubo.LinkToShader(defaultMaterial->GetShader());

        for(auto pair : materialRegistry)
            ubo.LinkToShader(pair.second->GetShader());
    }

    void MaterialManager::LinkSSBO(SSBO& ssbo) {
        ssbo.LinkToShader(defaultMaterial->GetShader());

        for(auto pair : materialRegistry)
            ssbo.LinkToShader(pair.second->GetShader());
    }

    MaterialManager::MaterialManager() : threadPool(MTL_MANAGER_THREAD_POOL_SIZE) {
        ActiveMaterialManager = this;
    }

    const std::function<void(OmicronMaterial*)>& MaterialManager::GetOnNewMaterial() const {
        return onNewMaterial;
    }

    void MaterialManager::SetOnNewMaterial(const std::function<void(OmicronMaterial*)>& onNewMaterial) {
        this->onNewMaterial = onNewMaterial;
    }

    void MaterialManager::Update(float delta) {
        for(auto mtl : materialRegistry)
            mtl.second->Update(delta);

        std::lock_guard<std::mutex> guard(materialQueueMutex);
        if(materialQueue.empty()) return;
        auto mtl = materialQueue.front();
        auto func = GetOnNewMaterial();
        if(func) func(mtl);
        materialQueue.pop();
    }

    OmicronMaterial *MaterialManager::GetMaterialInstance(std::string name, int instance, bool validOnly) {
//        if(instance <= -1)
            return GetMaterialBase(name, validOnly);

        if(!HasDynamicMaterial(name, instance))
            return GetMaterialBase(name, validOnly);


        auto mtl = dynamicMaterialRegistry[name][instance];

        if(validOnly && mtl->IsReady())
            return mtl;
        return defaultMaterial;
    }

    bool MaterialManager::HasDynamicMaterial(std::string name, int instance) {
//        if(!HasMaterial(name))
            return false;

        if(dynamicMaterialRegistry.find(name) == dynamicMaterialRegistry.end())
            return false;

        std::map<int, OmicronMaterial*> mtlReg = dynamicMaterialRegistry[name];

        if(mtlReg.find(instance) == mtlReg.end())
            return false;

        return true;
    }

    int MaterialManager::MakeDynamicMaterial(std::string name) {
//        if(!HasMaterial(name))
            return -2;

        if(dynamicMaterialRegistry.find(name) == dynamicMaterialRegistry.end()) {
            std::map<int, OmicronMaterial*> map;
            dynamicMaterialRegistry[name] = map;
        }

        int last = -1;
        if(!dynamicMaterialRegistry[name].empty())
            last = dynamicMaterialRegistry[name].rbegin()->first;

        int next = last + 1;
        dynamicMaterialRegistry[name][next] = new OmicronMaterial(GetMaterial(name));

        return next;
    }

    OmicronMaterial* MaterialManager::GetMaterial(std::string name, int instance, bool validOnly) {
        return GetMaterialBase(name, validOnly);
    }

    MaterialManager* MaterialManager::GetActiveMaterialManager() {
        return ActiveMaterialManager;
    }

    void MaterialManager::RemoveDynamic(std::string name, int instance) {
        if(!HasDynamicMaterial(name, instance))
            dynamicMaterialRegistry[name].erase(instance);

        if(dynamicMaterialRegistry[name].empty())
            dynamicMaterialRegistry.erase(name);

    }

}