//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_TRANSFORM_HPP
#define OMICRONRENDER_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mutex>

#define GUARD_SETTER(func, var, type, mutexVal) \
inline void func(type newValue) { \
    std::lock_guard<std::mutex> guard(mutexVal); \
    var = newValue; \
}

namespace Omicron {

    struct Transform {

        Transform() {}
        Transform(Transform& other) {
            Translation = other.Translation;
            Rotation = other.Rotation;
            Scale = other.Scale;
        }

        void operator=(Transform& other) {
            Translation = other.Translation;
            Rotation = other.Rotation;
            Scale = other.Scale;
        }

        glm::vec3 Translation = {0.f, 0.f, 0.f};
        glm::quat Rotation = {0.f, 0.f, 0.f, 1.f};
        glm::vec3 Scale = {1.f, 1.f, 1.f};

        bool useMatrix = true;

        inline glm::mat4 Model() {

            if(useMatrix) {
                glm::mat4 transMat = glm::translate(glm::mat4(), Translation);
                glm::mat4 rotMat = glm::mat4_cast(Rotation);
                glm::mat4 scaleMat = glm::scale(glm::mat4(), Scale);
                return transMat * rotMat * scaleMat;
            }

            glm::mat4 model;

            model = glm::translate(model, Translation);
            model = glm::rotate(model, Rotation.w, {Rotation.x, Rotation.y, Rotation.z});
            model = glm::scale(model, Scale);

            return model;

//            return glm::translate(glm::mat4(), Translation) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(), Scale);
        }

        GUARD_SETTER(SetTranslation, Translation, glm::vec3, TranslationMutex);
        GUARD_SETTER(SetRotation, Rotation, glm::quat, RotationMutex);
        GUARD_SETTER(SetScale, Scale, glm::vec3, ScaleMutex);

        inline void Translate(glm::vec3 offset) {
            glm::vec3 t(Translation);
            t += offset;
            SetTranslation(t);
        }

        inline void Rotate(glm::quat offset) {
            SetRotation(glm::rotate(Rotation, offset.w, {offset.x, offset.y, offset.z}));
        }

        std::mutex TranslationMutex;
        std::mutex RotationMutex;
        std::mutex ScaleMutex;

    };

};

#endif //OMICRONRENDER_TRANSFORM_HPP
