//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_MATERIALMANAGER_HPP
#define OMICRONRENDER_MATERIALMANAGER_HPP

#include <string>
#include <data/UBO.hpp>
#include <data/SSBO.hpp>
#include <ctpl_stl.h>
#include "OmicronMaterial.hpp"

#ifndef MTL_MANAGER_THREAD_POOL_SIZE
#define MTL_MANAGER_THREAD_POOL_SIZE 4
#endif

namespace Omicron {

    class MaterialManager {
    public:

        MaterialManager();


        OmicronMaterial* GetMaterial(std::string name, int instance = -1, bool validOnly = false);

        OmicronMaterial* GetMaterialBase(std::string name, bool validOnly = false);
        OmicronMaterial* GetMaterialInstance(std::string name, int instance, bool validOnly = false);
        void QueueMaterial(OmicronMaterial* mtl);
        void RegisterMaterial(OmicronMaterial* mtl);
        bool HasMaterial(std::string name);

        int MakeDynamicMaterial(std::string name);
        bool HasDynamicMaterial(std::string name, int instance);
        void RemoveDynamic(std::string name, int instance);

        void RegisterMaterials();
        void LinkUBO(UBO& ubo);
        void LinkSSBO(SSBO& ubo);

        void SetDefaultMaterial(OmicronMaterial* mtl);

        const std::function<void(OmicronMaterial*)>& GetOnNewMaterial() const;
        void SetOnNewMaterial(const std::function<void(OmicronMaterial*)>& onNewShader);

        void Update(float delta);

        OmicronMaterial* defaultMaterial;

        static MaterialManager* GetActiveMaterialManager();

    protected:
        std::mutex materialQueueMutex;
        std::queue<OmicronMaterial*> materialQueue;
        std::map<std::string, OmicronMaterial*> materialRegistry;
        std::map<std::string, std::map<int, OmicronMaterial*>> dynamicMaterialRegistry;
        ctpl::thread_pool threadPool;
        std::function<void(OmicronMaterial*)> onNewMaterial;
    };


}

#endif //OMICRONRENDER_MATERIALMANAGER_HPP
