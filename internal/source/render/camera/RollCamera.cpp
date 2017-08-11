//
// Created by Guy on 07/08/2017.
//

#include <render/camera/RollCamera.hpp>

namespace Omicron {

    void RollCamera::SetRotation(glm::quat rot) {
        this->rotation = rot;
        updateCameraVectors();
    }

    glm::quat RollCamera::GetRotation() {
        return this->rotation;
    }

    glm::mat4 RollCamera::GetViewMatrix() {
        return glm::translate(glm::mat4_cast(rotation), Position);
    }

}