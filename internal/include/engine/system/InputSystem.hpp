//
// Created by Guy on 19/08/2017.
//

#ifndef OMICRONRENDER_INPUTSYSTEM_HPP
#define OMICRONRENDER_INPUTSYSTEM_HPP

#include <script/IScriptable.hpp>
#include "OmicronSystem.hpp"

namespace Omicron {

    struct ButtonState {
        bool btnA = false;
        bool btnB = false;
        bool btnX = false;
        bool btnY = false;
        bool btnEnter = false;
        bool btnLThumb = false;
        bool btnRThumb = false;
    };

    struct TouchState {
        bool touchA = false;
        bool touchB = false;
        bool touchX = false;
        bool touchY = false;
        bool touchLThumb = false;
        bool touchRThumb = false;
        bool touchLThumbRest = false;
        bool touchRThumbRest = false;
        bool touchLThumbUp = false;
        bool touchRThumbUp = false;
    };

    struct TriggerTouchState {
        bool touchLIndexTrigger = false;
        bool touchRIndexTrigger = false;
        bool touchLIndexPointing = false;
        bool touchRIndexPointing = false;
    };

    struct TriggerState {
        float triggerLIndex;
        float triggerRIndex;
        float triggerLGrip;
        float triggerRGrip;

        float triggerLIndex_NoDeadzone;
        float triggerRIndex_NoDeadzone;
        float triggerLGrip_NoDeadzone;
        float triggerRGrip_NoDeadzone;

        float triggerLIndex_Raw;
        float triggerRIndex_Raw;
        float triggerLGrip_Raw;
        float triggerRGrip_Raw;
    };

    struct ThumbstickState {
        float thumbstickLX;
        float thumbstickLY;
        float thumbstickRX;
        float thumbstickRY;

        float thumbstickLX_NoDeadzone;
        float thumbstickLY_NoDeadzone;
        float thumbstickRX_NoDeadzone;
        float thumbstickRY_NoDeadzone;

        float thumbstickLX_Raw;
        float thumbstickLY_Raw;
        float thumbstickRX_Raw;
        float thumbstickRY_Raw;
    };

    struct InputState {
        ButtonState* buttons;
        TouchState* capacitive;
        TriggerTouchState* triggerCapacitive;
        TriggerState* triggers;
        ThumbstickState* thumbsticks;

        inline InputState() {
            buttons = new ButtonState;
            capacitive = new TouchState;
            triggerCapacitive = new TriggerTouchState;
            triggers = new TriggerState;
            thumbsticks = new ThumbstickState;
        }

        inline ButtonState       * Buttons()              { return buttons;               }
        inline TouchState        * Capacitive()           { return capacitive;            }
        inline TriggerTouchState * TriggerCapacitive()    { return triggerCapacitive;     }
        inline TriggerState      * Triggers()             { return triggers;              }
        inline ThumbstickState   * Thumbsticks()          { return thumbsticks;           }

    };

    class InputSystem : public OmicronSystem {
    public:
        InputSystem() : inputState(new InputState()) {}

        virtual void Update(float delta) override;
        virtual std::string Name() override;

        InputState* GetInputState();

    protected:
        void UpdateState();
        InputState* inputState;
    };

    class InputSystemAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(InputSystemAdapter)

}

#endif //OMICRONRENDER_INPUTSYSTEM_HPP
