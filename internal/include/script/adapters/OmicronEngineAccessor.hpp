//
// Created by Guy on 10/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENGINEADAPTER_HPP
#define OMICRONRENDER_OMICRONENGINEADAPTER_HPP

#include <vector>
#include <script/IScriptable.hpp>
#include <engine/system/InputSystem.hpp>
#include "OmicronEntityAccessor.hpp"

namespace Omicron {

    class OmicronEngine;

    struct OmicronEngineAccessor {
        explicit OmicronEngineAccessor();
        InputState* GetInputState(OmicronEngine* engine);

    };

    class OmicronEngineAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(OmicronEngineAdapter)

}

#endif //OMICRONRENDER_OMICRONENGINEADAPTER_HPP
