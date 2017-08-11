//
// Created by Guy on 09/08/2017.
//

#include <script/adapters/SystemAdapter.hpp>

namespace Omicron {

    void SystemAccessor::PrintMessage(std::string s) {
        printf("%s\n", s.c_str());
    }

    void SystemAdapter::Register(const sel::State& state) {
        printf("Registering system adapter\n");
        state["System"].SetClass<SystemAccessor>(
            "PrintMessage", &SystemAccessor::PrintMessage
        );
    }

    const char* SystemAdapter::Name() {
        return "SystemAdapter";
    }

}