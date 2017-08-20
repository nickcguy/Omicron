//
// Created by Guy on 10/08/2017.
//

#include <script/adapters/OmicronEntityAccessor.hpp>

namespace Omicron {

    OmicronEntityAccessor::OmicronEntityAccessor() {}

    OmicronEntityAccessor::OmicronEntityAccessor(OmicronEntity* entity) : entity(entity) {}

    std::string* OmicronEntityAccessor::GetTags() {
        return entity->GetTags().data();
    }

    std::string OmicronEntityAccessor::GetTagString() {
        return entity->TagString();
    }

    OmicronEntity* OmicronEntityAccessor::GetEntity() {
        return entity;
    }

    void OmicronEntityAdapter::Register(const sel::State& state) {
        state["Entity"].SetClass<OmicronEntity>(
            "GetComponentByName", &OmicronEntity::GetComponentByName,
            "HasTag", &OmicronEntity::HasTag,
            "HasAllTags", &OmicronEntity::HasAllTags,
            "AddTag", &OmicronEntity::AddTag,
            "RemoveTag", &OmicronEntity::RemoveTag,
            "TagString", &OmicronEntity::TagString,
            "GetTags", &OmicronEntity::GetTags,
            "GetTransform", &OmicronEntity::GetTransform
        );

        state["Transform"].SetClass<Transform>(
            "Translate", &Transform::Translate,
            "Rotate", &Transform::Rotate,
            "SetTranslation", &Transform::SetTranslation,
            "SetRotation", &Transform::SetRotation
        );

//        state["Entity"].SetClass<OmicronEntityAccessor>(
//            "GetTags", &OmicronEntityAccessor::GetTags,
//            "GetTagString", &OmicronEntityAccessor::GetTagString
//        );
    }

    const char* OmicronEntityAdapter::Name() {
        return "OmicronEntityAdapter";
    }

}