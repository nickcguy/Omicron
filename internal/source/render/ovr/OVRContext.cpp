//
// Created by Guy on 14/08/2017.
//

#include <render/ovr/OVRContext.hpp>
#include <render/newRender/FlatRenderer.hpp>
#include <render/newRender/OVRRenderer.hpp>
#include <utils/MathsUtils.hpp>
#include <engine/input/OVRInputProvider.hpp>

namespace Omicron {

    OVRContext::OVRContext(GLFWwindow* window, IRenderProvider* renderProvider) : OpenGLContext(window, renderProvider) {
    }



    void OVRContext::SpawnRenderer() {
        // TODO update to OVRRenderer
        renderer = new OVRRenderer(this);
    }

    void OVRContext::InitOVR() {
        if(OVR_FAILURE(ovr_Initialize(nullptr)))
            throw std::runtime_error("OVR context failed to initialise");

        if(OVR_FAILURE(ovr_Create(&session, &luid)))
            throw std::runtime_error("Failed to create OVR session");

        hmdDesc = ovr_GetHmdDesc(session);

        float pixelDensity = 1.0f;

        ovr_SetTrackingOriginType(session, ovrTrackingOrigin_FloorLevel);

        if(OVRRenderer* r = dynamic_cast<OVRRenderer*>(renderer))
            r->InitOVR();

        inputProvider = new OVRInputProvider;
        ovrInputProvider = dynamic_cast<OVRInputProvider*>(inputProvider);
    }

    void OVRContext::UpdateTrackingState() {
        trackingState = ovr_GetTrackingState(session, ovr_GetTimeInSeconds(), ovrTrue);
        ovrInputProvider->trackingState = trackingState;
        ovr_GetInputState(session, ovrControllerType_Touch, &ovrInputProvider->inputData);

        if(trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
            OVR::Posef headPose = trackingState.HeadPose.ThePose;
            auto headPos = headPose.Translation;
            headPos -= trackingState.CalibratedOrigin.Position;

            glm::vec3 pos = renderProvider->Position();
            headPos += EXTRACT_VECTOR(pos);

            camera->Position.x = headPos.x;
            camera->Position.y = headPos.y;
            camera->Position.z = headPos.z;

            auto headQuat = headPose.Rotation;
            headQuat.GetEulerAngles<OVR::Axis::Axis_X, OVR::Axis::Axis_Y, OVR::Axis::Axis_Z>(&camera->Yaw, &camera->Pitch, &camera->Roll);

            auto headMat3 = OVR::Matrix3f(headQuat);

            glm::vec3 f = EXTRACT_VECTOR(headMat3.Col(0));
            glm::vec3 u = EXTRACT_VECTOR(headMat3.Col(1));
            glm::vec3 r = EXTRACT_VECTOR(headMat3.Col(2));

            ovrInputProvider->forwardVector = r;
            ovrInputProvider->upVector = u;
            ovrInputProvider->rightVector = f;

            for(int hand = ovrHand_Left; hand <= ovrHand_Right; hand++) {
                if(handEntities[hand] && (trackingState.HandStatusFlags[hand] & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))) {
                    auto handPose = trackingState.HandPoses[hand].ThePose;
                    handEntities[hand]->transform.SetTranslation(Utils::ConvertVec3(handPose.Position) + Utils::ConvertVec3(headPos));
                    handEntities[hand]->transform.SetRotation(Utils::ConvertQuat(handPose.Orientation));
                }
            }
        }
    }

    ovrHmdStruct* OVRContext::GetSession() {
        return session;
    }

    ovrGraphicsLuid& OVRContext::GetLuid() {
        return luid;
    }

    ovrHmdDesc& OVRContext::GetHmdDesc() {
        return hmdDesc;
    }

    ovrTrackingState& OVRContext::GetTrackingState() {
        return trackingState;
    }

    void OVRContext::SpawnCamera() {
        camera = new RollCamera;
    }

    void OVRContext::Render(float delta) {
        UpdateTrackingState();
        OpenGLContext::Render(delta);
    }

    void OVRContext::SetHandEntity(ovrHandType hand, OmicronEntity* entity) {
        handEntities[hand] = entity;
    }


}