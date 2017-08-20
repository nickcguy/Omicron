//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_LIGHT_HPP
#define OMICRONRENDER_LIGHT_HPP

#include <glm/vec3.hpp>
#include <cstdio>
#include <atomic>
#include <mutex>

namespace Omicron {

    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOT,
        LightType_LENGTH
    };

    struct Light {
        LightType type;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 colour;
        double cutoff;
        double outerCutoff;
        double constant;
        double linear;
        double quadratic;
        double intensity;
        bool active = true;


        inline void ToggleActive() {
            active = !active;
        }

        inline bool IsActive() {
            return active;
        }


        inline Light() {}

        inline Light(Light* other) {
            this->type = other->type;
            this->position = other->position;
            this->direction = other->direction;
            this->colour = other->colour;
            this->cutoff = other->cutoff;
            this->outerCutoff = other->outerCutoff;
            this->constant = other->constant;
            this->linear = other->linear;
            this->quadratic = other->quadratic;
            this->intensity = other->intensity;
            this->active  = other->active;
        }

        LightType GetType() const {
            return type;
        }

        void SetType(LightType type) {
            Light::type = type;
        }

        const glm::vec3& GetPosition() const {
            return position;
        }

        void SetPosition(const glm::vec3& position) {
            Light::position = position;
        }

        const glm::vec3& GetDirection() const {
            return direction;
        }

        void SetDirection(const glm::vec3& direction) {
            Light::direction = direction;
        }

        const glm::vec3& GetColour() const {
            return colour;
        }

        void SetColour(const glm::vec3& colour) {
            Light::colour = colour;
        }

        float GetCutoff() const {
            return cutoff;
        }

        void SetCutoff(float cutoff) {
            Light::cutoff = cutoff;
        }

        float GetOuterCutoff() const {
            return outerCutoff;
        }

        void SetOuterCutoff(float outerCutoff) {
            Light::outerCutoff = outerCutoff;
        }

        float GetConstant() const {
            return constant;
        }

        void SetConstant(float constant) {
            Light::constant = constant;
        }

        float GetLinear() const {
            return linear;
        }

        void SetLinear(float linear) {
            Light::linear = linear;
        }

        float GetQuadratic() const {
            return quadratic;
        }

        void SetQuadratic(float quadratic) {
            Light::quadratic = quadratic;
        }

        double GetIntensity() const {
            return 1.0;
        }

        void SetIntensity(float intensity) {
            Light::intensity = intensity;
        }

        void SetActive(bool active) {
            Light::active = active;
        }

    };

}

#endif //OMICRONRENDER_LIGHT_HPP
