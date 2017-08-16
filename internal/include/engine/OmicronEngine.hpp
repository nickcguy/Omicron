//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENGINE_HPP
#define OMICRONRENDER_OMICRONENGINE_HPP

#include <chrono>
#include <engine/entity/OmicronEntity.hpp>
#include <mutex>
#include <utils/FilteredVector.hpp>
#include <engine/system/OmicronSystem.hpp>
#include <engine/input/InputProvider.hpp>
#include "render/IRenderProvider.hpp"

namespace Omicron {

    class OmicronEngineWrapper;

    struct DebugLine {
        glm::vec3 worldStart;
        glm::vec3 worldEnd;

        glm::vec3 startColour;
        glm::vec3 endColour;

        DebugLine(const glm::vec3& worldStart, const glm::vec3& worldEnd, const glm::vec3& startColour,
                  const glm::vec3& endColour, float life = 3.f) : worldStart(worldStart), worldEnd(worldEnd),
                                                            startColour(startColour), endColour(endColour),
                                                            life(life) {}

        float life = 3.f;

        inline RenderCommand ToRenderCommand() {
            RenderCommand cmd = {};
            cmd.type = VERTEX;
            cmd.material = "Line";
            cmd.primitive = GL_LINES;
            cmd.size = 12;
            cmd.offset = 0;

            std::vector<float> vertices(12);
            vertices[0]  = worldStart.x;
            vertices[1]  = worldStart.y;
            vertices[2]  = worldStart.z;

            vertices[3]  = startColour.r;
            vertices[4]  = startColour.g;
            vertices[5]  = startColour.b;

            vertices[6]  = worldEnd.x;
            vertices[7]  = worldEnd.y;
            vertices[8]  = worldEnd.z;

            vertices[9]  = endColour.r;
            vertices[10] = endColour.g;
            vertices[11] = endColour.b;

            glGenVertexArrays(1, &cmd.VAO);
            glGenBuffers(1, &cmd.VBO);
            glBindVertexArray(cmd.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, cmd.VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

            return cmd;
        }
    };

    class OmicronEngine : public IRenderProvider {
    public:
        explicit OmicronEngine(int targetIps, OmicronEngineWrapper* owner);

        OmicronEngine(OmicronEngine&) = delete;
        OmicronEngine(OmicronEngine&&) = delete;

        OmicronEngine(const OmicronEngine&) = delete;
        OmicronEngine(const OmicronEngine&&) = delete;

        void Init();
        void Loop(float delta);
        void Wait();
        void Step(float delta);

        void SetActive(bool active);
        bool IsActive();
        void Kill();

        void Renderables(std::vector<RenderCommand>& cmds) override;

        virtual void Lights(std::vector<Light>& vector) override;

        void SetTargetIPS(int targetIps);
        void CalculateInterval();

        Omicron::FilteredVector<OmicronEntity*> GetEntities();
        Omicron::FilteredVector<OmicronEntity*> GetTaggedEntities(std::string tag);
        Omicron::FilteredVector<OmicronEntity*> GetTaggedEntities_All(std::vector<std::string> tags);

        int EntityCount();

        template <typename T> inline Omicron::FilteredVector<OmicronEntity*> GetEntitiesWith() {
            FilteredVector<OmicronEntity*> entities;
            for(OmicronEntity* entity : this->entities) {
                if(entity->HasComponent<T>())
                    entities.push_back(entity);
            }
            return entities;
        }

        void AddEntity(OmicronEntity* entity);
        void AddTemplate(std::string name, OmicronEntity* entity);
        void RemoveEntity(OmicronEntity* entity);

        void AddSystem(OmicronSystem* system);
        inline OmicronSystem* GetSystem(std::string name) {
            for(auto sys : systems) {
                if(sys->Name() == name)
                    return sys;
            }
            return nullptr;
        }
        void RemoveSystem(OmicronSystem* system);

        void SetInputProvider(InputProvider* provider);
        InputProvider* GetProvider();
        InputProvider* inputProvider = nullptr;
        std::deque<DebugLine*> debugLines;

        OmicronEntity* UseTemplate(std::string name, bool addToWorld = true);

        void Clear();

    protected:
        /** Target Iterations per second */
        int targetIps;
        int currentIteration = 0;
        std::chrono::time_point<std::chrono::system_clock> nextStepTime;
        int stepMs;
        std::chrono::duration<int64_t, std::milli> stepDuration;
        bool active = true;
        bool alive = true;
        std::mutex activeMutex;
        std::mutex aliveMutex;
        OmicronEngineWrapper* owner;

        std::vector<OmicronEntity*> entities;
        std::map<std::string, OmicronEntity*> templateEntities;
        std::vector<OmicronSystem*> systems;


    };

}

#endif //OMICRONRENDER_OMICRONENGINE_HPP
