//
// Created by Guy on 08/08/2017.
//

#include <engine/component/PhysicsComponent.hpp>
#include <engine/entity/OmicronEntity.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <engine/system/OmicronMotionState.hpp>
#include <LinearMath/btDefaultMotionState.h>
#include <engine/component/MeshComponent.hpp>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <data/assimp/AssimpModel.hpp>

namespace Omicron {

    PhysicsComponent::PhysicsComponent(PhysicsSystem* world) : world(world) {


    }



    void PhysicsComponent::PullFrom() {

//        btVector3 pos = {owningEntity->transform.Translation.x, owningEntity->transform.Translation.y, owningEntity->transform.Translation.z};
//        glm::quat quat = owningEntity->transform.Rotation;
//        btQuaternion rot = {quat.w, quat.x, quat.y, quat.z};
//
//        btTransform transform(rot, pos);
//        body->setWorldTransform(transform);
    }

    void PhysicsComponent::PushTo() {
//        btTransform transform = GetBodyTransform();
//        btVector3 pos = transform.getOrigin();
//        btQuaternion rot = transform.getRotation();
//        owningEntity->transform.SetTranslation({pos.x(), pos.y(), pos.z()});
//        owningEntity->transform.SetRotation({rot.w(), rot.x(), rot.y(), rot.z()});
    }

    void PhysicsComponent::Destroy() {
//        delete[] shapes;
        world->GetWorld()->removeRigidBody(body);
        delete body;
    }



    void PhysicsComponent::AddCollisionShape(CollisionShape* shape, const PhysTransform& transform) {
        collisionShapes.push_back(ShapeDef{shape, transform});
    }

    void PhysicsComponent::SetType(BodyType type) {

    }

    void PhysicsComponent::AddToWorld(float mass) {

        btVector3 pos = {owningEntity->transform.Translation.x, owningEntity->transform.Translation.y,
                         owningEntity->transform.Translation.z};
        glm::quat quat = owningEntity->transform.Rotation;
        btQuaternion rot = {quat.w, quat.x, quat.y, quat.z};
        btTransform transform(rot, pos);

//        shape = new btCompoundShape;
//
//        for(auto item : collisionShapes)
//            ((btCompoundShape*)shape)->addChildShape(item.transform, item.shape);

        shape = collisionShapes[0].shape;

        bool isDynamic = (mass != 0.f);
        btVector3 localInertia(0, 0, 0);
        if(isDynamic)
            shape->calculateLocalInertia(mass, localInertia);

        btMotionState* motionState = new OmicronMotionState(owningEntity, transform);
//        btMotionState* motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo cInfo(mass, motionState, shape, localInertia);
        body = new btRigidBody(cInfo);
        body->setUserPointer(this);
        world->GetWorld()->addRigidBody(body);
    }

    PhysTransform PhysicsComponent::GetBodyTransform() {
        btTransform transform;
        if(body->getMotionState())
            body->getMotionState()->getWorldTransform(transform);
        else transform = body->getWorldTransform();

        return transform;
    }

    Body* PhysicsComponent::GetBody() {
        return body;
    }

    void PhysicsComponent::MatchEntityMesh() {
        if(!owningEntity) return;
        if(owningEntity->HasComponent<MeshComponent>()) {
            MeshComponent* mesh = owningEntity->GetCastComponent<MeshComponent>();
            std::vector<float> verts = mesh->ExtractVertices(owningEntity->transform.Scale);
            MatchEntityMesh(verts);
        }else if(owningEntity->HasComponent<ModelComponent>()) {
            ModelComponent* model = owningEntity->GetCastComponent<ModelComponent>();
            for(MeshComponent* mesh : model->meshes) {
                std::vector<float> verts = mesh->ExtractVertices(owningEntity->transform.Scale);
                MatchEntityMesh(verts);
            }
        }
    }

    void PhysicsComponent::MatchEntityMesh(std::vector<float> vertices) {
        btConvexHullShape* shape = new btConvexHullShape(vertices.data(), vertices.size()/3, sizeof(float) * 3);
        AddCollisionShape(shape);
    }

    OmicronComponent* PhysicsComponent::Clone() {
        return new PhysicsComponent(this);
    }

    PhysicsComponent::PhysicsComponent(PhysicsComponent* other) {
        mass = other->mass;
        collisionShapes = other->collisionShapes;
        world = other->world;
        AddToWorld(mass);
    }

    std::string PhysicsComponent::Name() {
        return "PhysicsComponent";
    }

    void PhysicsComponent::ClearForces() {
        GetBody()->clearForces();
    }

    void PhysicsComponent::SetGravity(glm::vec3 grav) {
        GetBody()->setGravity(btVector3(grav.x, grav.y, grav.z));
    }

    glm::vec3 PhysicsComponent::GetGravity() {
        btVector3 grav = GetBody()->getGravity();
        return glm::vec3(grav.x(), grav.y(), grav.z());
    }

    void PhysicsComponent::ApplyCentralForce(glm::vec3 force) {
        body->applyCentralForce(EXTRACT_VECTOR(force));
    }

    void PhysicsComponent::ApplyCentralImpulse(glm::vec3 force) {
        body->applyCentralImpulse(EXTRACT_VECTOR(force));
    }

    void PhysicsComponent::ApplyForce(glm::vec3 force, glm::vec3 relPos) {
        body->applyForce(EXTRACT_VECTOR(force), EXTRACT_VECTOR(relPos));
    }

    void PhysicsComponent::ApplyImpulse(glm::vec3 force, glm::vec3 relPos) {
        body->applyImpulse(EXTRACT_VECTOR(force), EXTRACT_VECTOR(relPos));
    }

    void PhysicsComponent::ApplyTorque(glm::vec3 torque) {
        body->applyTorque(EXTRACT_VECTOR(torque));
    }

    void PhysicsComponent::ApplyTorqueImpulse(glm::vec3 torque) {
        body->applyTorqueImpulse(EXTRACT_VECTOR(torque));
    }

    std::vector<float> PhysicsComponent::GetWorldPositionSet() {
        auto trans = GetBodyTransform();
        std::vector<float> pos(3);
        pos[0] = trans.getOrigin().x();
        pos[1] = trans.getOrigin().y();
        pos[2] = trans.getOrigin().z();
        return pos;
    }

    float PhysicsComponent::GetWorldPosition(int val) {
        return GetWorldPositionSet()[glm::clamp(val, 0, 2)];
    }

    PhysicsComponent::PhysicsComponent() {}

    BoundingBox PhysicsComponent::GetBoundingBox() {
        btVector3 min;
        btVector3 max;

        body->getAabb(min, max);
//        auto trans = GetBodyTransform();

//        min += trans.getOrigin();
//        max += trans.getOrigin();

        return BoundingBox{
            {min.x(), min.y(), min.z()},
            {max.x(), max.y(), max.z()}
        };
    }

}