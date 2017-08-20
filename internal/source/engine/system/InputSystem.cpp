//
// Created by Guy on 19/08/2017.
//

#include <engine/system/InputSystem.hpp>
#include <engine/OmicronEngine.hpp>
#include <engine/input/OVRInputProvider.hpp>

#define CHECK_BUTTON(btnMask, btnBool, eventName) { \
    bool state = inputData.Buttons & (btnMask); \
    if(state != btnBool) { \
        btnBool = state; \
        handler->PostEvent(EngineEvent(eventName, PrimitiveVariant(state))); \
    } \
}

#define CHECK_TOUCH(touchMask, touchBool, eventName) { \
    bool state = inputData.Touches & (touchMask); \
    if(state != touchBool) { \
        touchBool = state; \
        handler->PostEvent(EngineEvent(eventName, PrimitiveVariant(state))); \
    } \
}

namespace Omicron {

    void InputSystem::Update(float delta) {
        UpdateState();
    }

    std::string InputSystem::Name() {
        return "InputSystem";
    }

    // TODO find a better way to do this
    void InputSystem::UpdateState() {
        if(!engine->inputProvider) return;
        if(engine->inputProvider->GetType() != OVR) return;
        OVRInputProvider* provider = static_cast<OVRInputProvider*>(engine->inputProvider);
        auto inputData = provider->inputData;

        auto handler = engine->GetEventHandler();

//        if(inputData.Buttons & ovrButton_A != inputState.buttons.btnA) {
//            inputState.buttons.btnA = inputData.Buttons & ovrButton_A;
//            engine->GetEventHandler().PostEvent(EngineEvent("ovrbutton_AChange", {inputState.buttons.btnA}));
//        }

        // ButtonState
        CHECK_BUTTON(ovrButton_A, inputState->buttons->btnA, "ovrButton_AChange");
        CHECK_BUTTON(ovrButton_B, inputState->buttons->btnB, "ovrButton_BChange");
        CHECK_BUTTON(ovrButton_X, inputState->buttons->btnX, "ovrButton_XChange");
        CHECK_BUTTON(ovrButton_Y, inputState->buttons->btnY, "ovrButton_YChange");
        CHECK_BUTTON(ovrButton_Enter, inputState->buttons->btnEnter, "ovrButton_EnterChange");
        CHECK_BUTTON(ovrButton_LThumb, inputState->buttons->btnLThumb, "ovrButton_LThumbChange");
        CHECK_BUTTON(ovrButton_RThumb, inputState->buttons->btnRThumb, "ovrButton_RThumbChange");

        // TouchState
        CHECK_TOUCH(ovrTouch_A, inputState->capacitive->touchA, "ovrTouch_AChange");
        CHECK_TOUCH(ovrTouch_B, inputState->capacitive->touchB, "ovrTouch_BChange");
        CHECK_TOUCH(ovrTouch_X, inputState->capacitive->touchX, "ovrTouch_XChange");
        CHECK_TOUCH(ovrTouch_Y, inputState->capacitive->touchY, "ovrTouch_YChange");
        CHECK_TOUCH(ovrTouch_LThumb, inputState->capacitive->touchLThumb, "ovrTouch_LThumbChange");
        CHECK_TOUCH(ovrTouch_RThumb, inputState->capacitive->touchRThumb, "ovrTouch_RThumbChange");
        CHECK_TOUCH(ovrTouch_LThumbRest, inputState->capacitive->touchLThumbRest, "ovrTouch_LThumbRestChange");
        CHECK_TOUCH(ovrTouch_RThumbRest, inputState->capacitive->touchRThumbRest, "ovrTouch_RThumbRestChange");
        CHECK_TOUCH(ovrTouch_LThumbUp, inputState->capacitive->touchLThumbUp, "ovrTouch_LThumbUpChange");
        CHECK_TOUCH(ovrTouch_RThumbUp, inputState->capacitive->touchRThumbUp, "ovrTouch_RThumbUpChange");

        // TriggerTouchState
        CHECK_TOUCH(ovrTouch_LIndexTrigger, inputState->triggerCapacitive->touchLIndexTrigger, "ovrTouch_LIndexTriggerChange");
        CHECK_TOUCH(ovrTouch_RIndexTrigger, inputState->triggerCapacitive->touchRIndexTrigger, "ovrTouch_RIndexTriggerChange");
        CHECK_TOUCH(ovrTouch_LIndexPointing, inputState->triggerCapacitive->touchLIndexPointing, "ovrTouch_LIndexPointingChange");
        CHECK_TOUCH(ovrTouch_RIndexPointing, inputState->triggerCapacitive->touchRIndexPointing, "ovrTouch_RIndexPointingChange");

        // TriggerState
        inputState->triggers->triggerLIndex             = inputData.IndexTrigger[ovrHand_Left];
        inputState->triggers->triggerRIndex             = inputData.IndexTrigger[ovrHand_Right];
        inputState->triggers->triggerLIndex_NoDeadzone  = inputData.IndexTriggerNoDeadzone[ovrHand_Left];
        inputState->triggers->triggerRIndex_NoDeadzone  = inputData.IndexTriggerNoDeadzone[ovrHand_Right];
        inputState->triggers->triggerLIndex_Raw         = inputData.IndexTriggerRaw[ovrHand_Left];
        inputState->triggers->triggerRIndex_Raw         = inputData.IndexTriggerRaw[ovrHand_Right];

        inputState->triggers->triggerLGrip              = inputData.HandTrigger[ovrHand_Left];
        inputState->triggers->triggerRGrip              = inputData.HandTrigger[ovrHand_Right];
        inputState->triggers->triggerLGrip_NoDeadzone   = inputData.HandTriggerNoDeadzone[ovrHand_Left];
        inputState->triggers->triggerRGrip_NoDeadzone   = inputData.HandTriggerNoDeadzone[ovrHand_Right];
        inputState->triggers->triggerLGrip_Raw          = inputData.HandTriggerRaw[ovrHand_Left];
        inputState->triggers->triggerRGrip_Raw          = inputData.HandTriggerRaw[ovrHand_Right];

        // ThumbstickState
        inputState->thumbsticks->thumbstickLX             = inputData.Thumbstick[ovrHand_Left].x;
        inputState->thumbsticks->thumbstickRX             = inputData.Thumbstick[ovrHand_Right].x;
        inputState->thumbsticks->thumbstickLX_NoDeadzone  = inputData.ThumbstickNoDeadzone[ovrHand_Left].x;
        inputState->thumbsticks->thumbstickRX_NoDeadzone  = inputData.ThumbstickNoDeadzone[ovrHand_Right].x;
        inputState->thumbsticks->thumbstickLX_Raw         = inputData.ThumbstickRaw[ovrHand_Left].x;
        inputState->thumbsticks->thumbstickRX_Raw         = inputData.ThumbstickRaw[ovrHand_Right].x;

        inputState->thumbsticks->thumbstickLY             = inputData.Thumbstick[ovrHand_Left].y;
        inputState->thumbsticks->thumbstickRY             = inputData.Thumbstick[ovrHand_Right].y;
        inputState->thumbsticks->thumbstickLY_NoDeadzone  = inputData.ThumbstickNoDeadzone[ovrHand_Left].y;
        inputState->thumbsticks->thumbstickRY_NoDeadzone  = inputData.ThumbstickNoDeadzone[ovrHand_Right].y;
        inputState->thumbsticks->thumbstickLY_Raw         = inputData.ThumbstickRaw[ovrHand_Left].y;
        inputState->thumbsticks->thumbstickRY_Raw         = inputData.ThumbstickRaw[ovrHand_Right].y;

    }

    InputState* InputSystem::GetInputState() {
        return inputState;
    }

    void InputSystemAdapter::Register(const sel::State& state) {
        state["Input"].SetClass<InputState>(
            "Buttons", &InputState::Buttons,
            "Capacitive", &InputState::Capacitive,
            "Touches", &InputState::Capacitive,
            "TriggerCapacitive", &InputState::TriggerCapacitive,
            "TriggerTouches", &InputState::TriggerCapacitive,
            "Triggers", &InputState::Triggers,
            "Thumbsticks", &InputState::Thumbsticks
        );

        state["ButtonState"].SetClass<ButtonState>(
            "btnA", &ButtonState::btnA,
            "btnB", &ButtonState::btnB,
            "btnX", &ButtonState::btnX,
            "btnY", &ButtonState::btnY,
            "btnEnter", &ButtonState::btnEnter,
            "btnLThumb", &ButtonState::btnLThumb,
            "btnRThumb", &ButtonState::btnRThumb
        );

        state["TouchState"].SetClass<TouchState>(
            "touchA", &TouchState::touchA,
            "touchB", &TouchState::touchB,
            "touchX", &TouchState::touchX,
            "touchY", &TouchState::touchY,
            "touchLThumb", &TouchState::touchLThumb,
            "touchRThumb", &TouchState::touchRThumb,
            "touchLThumbRest", &TouchState::touchLThumbRest,
            "touchRThumbRest", &TouchState::touchRThumbRest,
            "touchLThumbUp", &TouchState::touchLThumbUp,
            "touchRThumbUp", &TouchState::touchRThumbUp
        );

        state["TriggerTouchState"].SetClass<TriggerTouchState>(
            "touchLIndexTrigger", &TriggerTouchState::touchLIndexTrigger,
            "touchRIndexTrigger", &TriggerTouchState::touchRIndexTrigger,
            "touchLIndexPointing", &TriggerTouchState::touchLIndexPointing,
            "touchRIndexPointing", &TriggerTouchState::touchRIndexPointing
        );

        state["TriggerState"].SetClass<TriggerState>(
            "triggerLIndex", &TriggerState::triggerLIndex,
            "triggerRIndex", &TriggerState::triggerRIndex,
            "triggerLGrip", &TriggerState::triggerLGrip,
            "triggerRGrip", &TriggerState::triggerRGrip,
            "triggerLIndex_NoDeadzone", &TriggerState::triggerLIndex_NoDeadzone,
            "triggerRIndex_NoDeadzone", &TriggerState::triggerRIndex_NoDeadzone,
            "triggerLGrip_NoDeadzone", &TriggerState::triggerLGrip_NoDeadzone,
            "triggerRGrip_NoDeadzone", &TriggerState::triggerRGrip_NoDeadzone,
            "triggerLIndex_Raw", &TriggerState::triggerLIndex_Raw,
            "triggerRIndex_Raw", &TriggerState::triggerRIndex_Raw,
            "triggerLGrip_Raw", &TriggerState::triggerLGrip_Raw,
            "triggerRGrip_Raw", &TriggerState::triggerRGrip_Raw
        );

        state["ThumbstickState"].SetClass<ThumbstickState>(
            "thumbstickLX", &ThumbstickState::thumbstickLX,
            "thumbstickLY", &ThumbstickState::thumbstickLY,
            "thumbstickRX", &ThumbstickState::thumbstickRX,
            "thumbstickRY", &ThumbstickState::thumbstickRY,
            "thumbstickLX_NoDeadzone", &ThumbstickState::thumbstickLX_NoDeadzone,
            "thumbstickLY_NoDeadzone", &ThumbstickState::thumbstickLY_NoDeadzone,
            "thumbstickRX_NoDeadzone", &ThumbstickState::thumbstickRX_NoDeadzone,
            "thumbstickRY_NoDeadzone", &ThumbstickState::thumbstickRY_NoDeadzone,
            "thumbstickLX_Raw", &ThumbstickState::thumbstickLX_Raw,
            "thumbstickLY_Raw", &ThumbstickState::thumbstickLY_Raw,
            "thumbstickRX_Raw", &ThumbstickState::thumbstickRX_Raw,
            "thumbstickRY_Raw", &ThumbstickState::thumbstickRY_Raw
        );

    }

    const char* InputSystemAdapter::Name() {
        return "InputSystemAdapter";
    }
}