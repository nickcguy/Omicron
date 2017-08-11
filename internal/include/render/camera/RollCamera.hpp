//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_QUATCAMERA_HPP
#define OMICRONRENDER_QUATCAMERA_HPP

#include <glm/gtc/quaternion.hpp>
#include "Camera.hpp"

namespace Omicron {

    class RollCamera : public Camera {
    public:
        void SetRotation(glm::quat rot);

        virtual glm::mat4 GetViewMatrix() override;

        glm::quat GetRotation();
    protected:

        glm::quat rotation;
    };

}

#endif //OMICRONRENDER_QUATCAMERA_HPP
