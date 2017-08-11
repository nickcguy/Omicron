//
// Created by Guy on 08/08/2017.
//

#include <engine/system/OmicronMotionState.hpp>

namespace Omicron {


    OmicronMotionState::OmicronMotionState(OmicronEntity* entity, const btTransform& initialPosition)
        : entity(entity), initialPosition(initialPosition) {}

    void OmicronMotionState::SetEntity(OmicronEntity* entity) {
        this->entity = entity;
    }

    void OmicronMotionState::getWorldTransform(btTransform& worldTrans) const {
        if(entity == nullptr) {
            worldTrans = initialPosition;
            return;
        }
        btVector3 pos = {entity->transform.Translation.x, entity->transform.Translation.y, entity->transform.Translation.z};
        glm::quat quat = entity->transform.Rotation;
        btQuaternion rot = {quat.w, quat.x, quat.y, quat.z};
        btTransform trans(rot, pos);
        worldTrans = trans;
    }

    void OmicronMotionState::setWorldTransform(const btTransform& worldTrans) {
        if(entity == nullptr) return;

        btQuaternion rot = worldTrans.getRotation();
        btVector3 pos = worldTrans.getOrigin();

        entity->transform.SetTranslation({pos.x(), pos.y(), pos.z()});

        entity->transform.SetRotation({rot.w(), rot.x(), rot.y(), rot.z()});
    }

    OmicronMotionState::~OmicronMotionState() = default;
}