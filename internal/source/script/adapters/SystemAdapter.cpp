//
// Created by Guy on 09/08/2017.
//

#include <script/adapters/SystemAdapter.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Omicron {

    void SystemAccessor::PrintMessage(std::string s) {
        printf("%s\n", s.c_str());
    }

    float SystemAccessor::DegToRad(float deg) {
        return glm::radians(deg);
    }

    float SystemAccessor::RadToDeg(float rad) {
        return glm::degrees(rad);
    }

    void SystemAdapter::Register(const sel::State& state) {
        printf("Registering system adapter\n");
        state["System"].SetClass<SystemAccessor>(
            "PrintMessage", &SystemAccessor::PrintMessage,
            "DegToRad", &SystemAccessor::DegToRad,
            "RadToDeg", &SystemAccessor::RadToDeg
        );

        state["Vector3"].SetClass<glm::vec3>(
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );

        state["Quaternion"].SetClass<glm::quat>(
            "x", &glm::quat::x,
            "y", &glm::quat::y,
            "z", &glm::quat::z,
            "w", &glm::quat::w
        );
    }

    const char* SystemAdapter::Name() {
        return "SystemAdapter";
    }

}