//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_PHYSICSSYSTEM_HPP
#define OMICRONRENDER_PHYSICSSYSTEM_HPP

#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <glm/vec3.hpp>
#include "OmicronSystem.hpp"

namespace Omicron {

    typedef btDiscreteDynamicsWorld WorldType;

    class PhysicsSystem : public OmicronSystem {
    public:

        PhysicsSystem();
        explicit PhysicsSystem(glm::vec3 gravity);
        PhysicsSystem(float x, float y, float z);

        virtual void Destroy() override;

        virtual void Update(float delta) override;

        WorldType* GetWorld();

        virtual std::string Name() override;

    protected:
        WorldType* world;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btBroadphaseInterface* overlappingPairCache;
        btSequentialImpulseConstraintSolver* solver;
    };

}

#endif //OMICRONRENDER_PHYSICSSYSTEM_HPP
