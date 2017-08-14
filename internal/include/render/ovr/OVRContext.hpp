//
// Created by Guy on 14/08/2017.
//

#ifndef OMICRONRENDER_OVRCONTEXT_HPP
#define OMICRONRENDER_OVRCONTEXT_HPP

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
    protected:
        void UpdateTrackingState();

        ovrSession session;
        ovrGraphicsLuid luid;
        ovrHmdDesc hmdDesc;
        ovrTrackingState trackingState;

    };

}

#endif //OMICRONRENDER_OVRCONTEXT_HPP
