//
// Created by Guy on 08/08/2017.
//

#include <engine/system/PhysicsSystem.hpp>
#include <engine/component/PhysicsComponent.hpp>
#include <engine/OmicronEngine.hpp>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

namespace Omicron {

    PhysicsSystem::PhysicsSystem() : PhysicsSystem(0.f, -9.81f, 0.f) {}

    PhysicsSystem::PhysicsSystem(glm::vec3 gravity) : PhysicsSystem(gravity.x, gravity.y, gravity.z) {}

    PhysicsSystem::PhysicsSystem(float x, float y, float z) {

        collisionConfiguration = new btDefaultCollisionConfiguration;
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase;
        solver = new btSequentialImpulseConstraintSolver;

        world = new WorldType(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        world->setGravity(btVector3(x, y, z));
    }

    WorldType* PhysicsSystem::GetWorld() {
        return world;
    }

    void PhysicsSystem::Update(float delta) {
        auto entities = engine->GetEntitiesWith<PhysicsComponent>();
        world->stepSimulation(delta, 10);
    }

    void PhysicsSystem::Destroy() {
        OmicronSystem::Destroy();

        for(int i = world->getNumCollisionObjects()-1; i >= 0; i--) {
            btCollisionObject* obj = world->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if(body && body->getMotionState())
                delete body->getMotionState();
            world->removeCollisionObject(obj);
            delete obj;
        }

        CLEAR_PTR(world);
        CLEAR_PTR(solver);
        CLEAR_PTR(overlappingPairCache);
        CLEAR_PTR(dispatcher);
        CLEAR_PTR(collisionConfiguration);

    }

    std::string PhysicsSystem::Name() {
        return "Physics";
    }

}