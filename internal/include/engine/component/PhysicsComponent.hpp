//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_PHYSICSCOMPONENT_HPP
#define OMICRONRENDER_PHYSICSCOMPONENT_HPP

#include "OmicronComponent.hpp"
#include <engine/system/PhysicsSystem.hpp>
#include <vector>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>


namespace Omicron {

//    typedef btCollisionShape CollisionShape;
//    typedef btTransform PhysTransform;
//    typedef btRigidBody Body;
//    typedef btCollisionObject::CollisionFlags BodyType;

    #define CollisionShape btCollisionShape
    #define PhysTransform btTransform
    #define Body btRigidBody
    #define BodyType btCollisionObject::CollisionFlags

    struct ShapeDef {
        CollisionShape* shape;
        PhysTransform transform;
    };

    class PhysicsComponent : public OmicronComponent {
    public:
        explicit PhysicsComponent(PhysicsSystem* world);
        explicit PhysicsComponent(PhysicsComponent* other);

        virtual OmicronComponent* Clone() override;

        void PullFrom();
        void PushTo();

        void Destroy();

        void MatchEntityMesh();
        void MatchEntityMesh(std::vector<float> vertices);
        void AddCollisionShape(CollisionShape* shape, const PhysTransform& transform = PhysTransform());
        void AddToWorld(float mass = 1.0f);
        void SetType(BodyType type);

        PhysTransform GetBodyTransform();
        Body* GetBody();

        virtual std::string Name() override;

    protected:
        btCollisionShape* shape;
        float mass;
        PhysicsSystem* world;
        Body* body;
        std::vector<ShapeDef> collisionShapes;
    };

}

#endif //OMICRONRENDER_PHYSICSCOMPONENT_HPP
