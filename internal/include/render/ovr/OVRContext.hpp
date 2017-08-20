//
// Created by Guy on 14/08/2017.
//

#ifndef OMICRONRENDER_OVRCONTEXT_HPP
#define OMICRONRENDER_OVRCONTEXT_HPP

#include <engine/input/OVRInputProvider.hpp>
#include "../OpenGLContext.hpp"

namespace Omicron {

    class OVRContext : public OpenGLContext {
    public:
        OVRContext(GLFWwindow* window, IRenderProvider* renderProvider);

        void InitOVR();

        virtual void SpawnRenderer() override;
        virtual void SpawnCamera() override;

        virtual void Render(float delta) override;

        ovrHmdStruct* GetSession();
        ovrGraphicsLuid& GetLuid();
        ovrHmdDesc& GetHmdDesc();
        ovrTrackingState& GetTrackingState();

        glm::vec3 worldPosition = glm::vec3(0.f);

        void SetHandEntity(ovrHandType_ type_, OmicronEntity* pEntity);

    protected:
        void UpdateTrackingState();

        ovrSession session;
        ovrGraphicsLuid luid;
        ovrHmdDesc hmdDesc;
        ovrTrackingState trackingState;

        OmicronEntity* handEntities[2] = {nullptr, nullptr};
        OVRInputProvider* ovrInputProvider = nullptr;
    };

}

#endif //OMICRONRENDER_OVRCONTEXT_HPP
