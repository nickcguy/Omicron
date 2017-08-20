//
// Created by Guy on 06/08/2017.
//

#include <pch.hpp>
#include <engine/OmicronEngine.hpp>
#include <thread>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <engine/OmicronEngineWrapper.hpp>
#include <utils/FilteredVector.hpp>
#include <engine/component/MeshComponent.hpp>
#include <algorithm>
#include <engine/component/PointCloudComponent.hpp>
#include <engine/component/MaterialComponent.hpp>
#include <engine/input/OVRInputProvider.hpp>
#include <script/ScriptHost.hpp>
#include <data/assimp/AssimpModel.hpp>
#include <engine/component/LightComponent.hpp>
#include <engine/component/PhysicsComponent.hpp>

namespace Omicron {

    OmicronEngine::OmicronEngine(int targetIps, OmicronEngineWrapper* owner) : targetIps(glm::clamp(targetIps, 1, 1000)), owner(owner), eventHandler(new EngineEventHandler(this)) {
        if(owner)
            owner->SetChild(this);
    }

    void OmicronEngine::Init() {
        CalculateInterval();
        nextStepTime = std::chrono::system_clock::now() + stepDuration;
        DELTA_TIMER_DEC
        while(alive) {
            DELTA_TIMER_DEF
            Loop(delta);
        }

        CLEAR_PTR(eventHandler);
    }

    void OmicronEngine::Loop(float delta) {
        if(IsActive()) Step(delta);
        Wait();
    }

    void OmicronEngine::Wait() {
        std::this_thread::sleep_until(nextStepTime);
        nextStepTime += stepDuration;
    }

    bool executed = false;

    void OmicronEngine::Step(float delta) {
        currentIteration++;

        eventHandler->Update();

        for(auto system : systems)
            if(system->IsActive())
            system->Update(delta);

        for(DebugLine* line : debugLines) {
            if(line) {
                line->life -= delta;
                if(line->life <= 0.f) {
                    debugLines.erase(std::remove(debugLines.begin(), debugLines.end(), line), debugLines.end());
                }
            }
        }
    }

    void OmicronEngine::SetActive(bool active) {
        std::lock_guard<std::mutex> guard(activeMutex);
        this->active = active;
    }

    bool OmicronEngine::IsActive() {
        std::lock_guard<std::mutex> guard(activeMutex);
        return this->active;
    }

    void OmicronEngine::Kill() {
        printf("Kill signal received\n");
        std::lock_guard<std::mutex> guard(aliveMutex);
        alive = false;
    }

    void OmicronEngine::SetTargetIPS(int targetIps) {
        this->targetIps = targetIps;
        CalculateInterval();
    }

    void OmicronEngine::CalculateInterval() {
        stepMs = 1000 / targetIps;
        stepDuration = std::chrono::milliseconds(stepMs);
    }

    void OmicronEngine::Renderables(std::vector<RenderCommand>& cmds) {
        FilteredVector<OmicronEntity*> entities = GetEntitiesWith<MeshComponent>();
        for(OmicronEntity* entity : entities) {
            MeshComponent* comp = entity->GetCastComponent<MeshComponent>();

            glm::mat4 model = entity->transform.Model();
            RenderCommand cmd;
            if(comp->type == INDEXED) {
                cmd = CommandFactory::BuildIndexCommand(
                    comp->VAO,
                    comp->VBO,
                    comp->EBO,
                    model,
                    comp->size,
                    comp->offset,
                    comp->primitive
                );
            }else{
                cmd = CommandFactory::BuildVertexCommand(
                /* VAO */        comp->VAO,
                /* VBO */        comp->VBO,
                /* model */      model,
                /* size */       comp->size,
                /* offset */     comp->offset,
                /* primitive */  comp->primitive
                );
            }

            MaterialComponent* mtlComp;

            if(entity->HasComponent<MaterialComponent>()) mtlComp = entity->GetCastComponent<MaterialComponent>();
            else if(comp->materialComponent) mtlComp = comp->materialComponent;

            if(mtlComp) {
                cmd.material = mtlComp->materialId;
                cmd.uniforms = mtlComp->uniforms;
                cmd.alpha = mtlComp->alpha;
            }else{
                cmd.alpha = 1.f;
            }

            cmd.uniforms["alpha"] = cmd.alpha;

            cmds.push_back(cmd);
        }

        FilteredVector<OmicronEntity*> modelEntities = GetEntitiesWith<ModelComponent>();
        for(OmicronEntity* entity : modelEntities) {
            ModelComponent* modelComp = entity->GetCastComponent<ModelComponent>();
            glm::mat4 model = entity->transform.Model();
            for(auto mesh : modelComp->meshes) {

                RenderCommand cmd;
                if(mesh->type == INDEXED) {
                    cmd = CommandFactory::BuildIndexCommand(
                    mesh->VAO,
                    mesh->VBO,
                    mesh->EBO,
                    model,
                    mesh->size,
                    mesh->offset,
                    mesh->primitive
                    );
                }else{
                    cmd = CommandFactory::BuildVertexCommand(
                    /* VAO */        mesh->VAO,
                    /* VBO */        mesh->VBO,
                    /* model */      model,
                    /* size */       mesh->size,
                    /* offset */     mesh->offset,
                    /* primitive */  mesh->primitive
                    );
                }

                if(entity->HasComponent<MaterialComponent>()) {
                    MaterialComponent* mtlComp = entity->GetCastComponent<MaterialComponent>();
                    cmd.material = mtlComp->materialId;
                    cmd.uniforms = mtlComp->uniforms;
                    cmd.alpha = mtlComp->alpha;
                    cmd.uniforms["alpha"] = cmd.alpha;
                }

                cmds.push_back(cmd);
            }
        }

        FilteredVector<OmicronEntity*> pointCloud = GetEntitiesWith<PointCloudComponent>();
        for(OmicronEntity* entity : pointCloud) {
            PointCloudComponent* comp = entity->GetCastComponent<PointCloudComponent>();

            glm::mat4 model = entity->transform.Model();

            RenderCommand cmd = CommandFactory::BuildPointCloud(
            /* VAO */        comp->VAO,
            /* VBO */        comp->VBO,
            /* model */      model,
            /* size */       comp->size,
            /* offset */     comp->offset,
            /* primitive */  GL_POINTS
            );

            cmds.push_back(cmd);
        }

        for(DebugLine* line : debugLines)
            if(line) cmds.push_back(line->ToRenderCommand());

    }

    FilteredVector<OmicronEntity*> OmicronEngine::GetEntities() {
        FilteredVector<OmicronEntity*> entities;
        for(OmicronEntity* entity : this->entities)
            entities.push_back(entity);
        return entities;
    }

    void OmicronEngine::AddEntity(OmicronEntity* entity) {
        entities.push_back(entity);
    }

    void OmicronEngine::RemoveEntity(OmicronEntity* entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    void OmicronEngine::AddSystem(OmicronSystem* system) {
        systems.push_back(system);
        system->SetEngine(this);
    }

    void OmicronEngine::RemoveSystem(OmicronSystem* system) {
        systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
    }

    void OmicronEngine::SetInputProvider(InputProvider* provider) {
        this->inputProvider = provider;
    }

    InputProvider* OmicronEngine::GetProvider() {
        return this->inputProvider;
    }

    void OmicronEngine::Clear() {
        SetActive(false);
        entities.clear();
        for(auto sys : systems)
            sys->Destroy();
        systems.clear();
        SetActive(true);
    }

    Omicron::FilteredVector<OmicronEntity*> OmicronEngine::GetTaggedEntities(std::string tag) {
        FilteredVector<OmicronEntity*> entities;

        for(auto e : this->entities)
            if(e->HasTag(tag)) entities.push_back(e);

        return entities;
    }

    Omicron::FilteredVector<OmicronEntity*> OmicronEngine::GetTaggedEntities_All(std::vector<std::string> tags) {
        FilteredVector<OmicronEntity*> entities;

        for(auto e : this->entities)
            if(e->HasAllTags(tags)) entities.push_back(e);

        return entities;
    }

    OmicronEntity* OmicronEngine::UseTemplate(std::string name, bool addToWorld) {
        if(templateEntities.find(name) != templateEntities.end()) {
            OmicronEntity* entity = new OmicronEntity(templateEntities[name]);
            if(addToWorld) AddEntity(entity);
            return entity;
        }
        return nullptr;
    }

    void OmicronEngine::AddTemplate(std::string name, OmicronEntity* entity) {
        if(templateEntities.find(name) != templateEntities.end()) {
            CLEAR_PTR(templateEntities[name]);
        }
        templateEntities[name] = entity;
    }

    int OmicronEngine::EntityCount() {
        return entities.size();
    }

    void OmicronEngine::Lights(std::vector<Light*>& vector) {
        FilteredVector<OmicronEntity*> entities = GetEntitiesWith<LightComponent>();
        LightComponent* comp = nullptr;
        for(auto entity : entities) {
            comp = entity->GetCastComponent<LightComponent>();
            Light* l = new Light(comp->lightData);
            l->position += entity->transform.Translation;
            l->direction = -glm::mat3_cast(entity->transform.Rotation)[2];
            vector.push_back(l);
        }
    }

    int OmicronEngine::GetTaggedCount(std::string tag) {
        return GetTaggedEntities(tag).size();
    }

    EngineEventHandler* OmicronEngine::GetEventHandler() const {
        return eventHandler;
    }

    glm::vec3 OmicronEngine::Position() {
        return OriginPosition;
    }

    double SquaredDistPointAABB( const glm::vec3 & p, const BoundingBox& aabb ) {
        auto check = [&](
        const double pn,
        const double bmin,
        const double bmax) -> double {
            double out = 0;
            double v = pn;

            if ( v < bmin ) {
                double val = (bmin - v);
                out += val * val;
            }

            if ( v > bmax ) {
                double val = (v - bmax);
                out += val * val;
            }

            return out;
        };

        // Squared distance
        double sq = 0.0;

        sq += check( p.x, aabb.min.x, aabb.max.x );
        sq += check( p.y, aabb.min.y, aabb.max.y );
        sq += check( p.z, aabb.min.z, aabb.max.z );

        return sq;
    }

    bool TestSphereAABB(glm::vec3 point, float radius, const BoundingBox& aabb ) {
        return SquaredDistPointAABB(point, aabb) <= (radius * radius);
    }

    FilteredVector<OmicronEntity*> OmicronEngine::GetPhysicalEntitiesInRange(glm::vec3 point, float range) {
        FilteredVector<OmicronEntity*> entities;

        for(auto e : GetEntitiesWith<PhysicsComponent>()) {
            PhysicsComponent* phys = e->GetCastComponent<PhysicsComponent>();
            auto box = phys->GetBoundingBox();

            if(TestSphereAABB(point, range, box)) {
                entities.push_back(e);
            }
        }

        return entities;
    }

};