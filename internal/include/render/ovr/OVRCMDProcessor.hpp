//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_OVRCMDPROCESSOR_HPP
#define OMICRONRENDER_OVRCMDPROCESSOR_HPP

#include <data/Framebuffer.hpp>
#include <render/RenderCommand.hpp>
#include <render/OpenGLRenderer.hpp>
#include <Extras/OVR_Math.h>


namespace Omicron {

    namespace ovr {

        class OVRRenderer;

        class OVRCMDProcessor {
        public:
            explicit OVRCMDProcessor(OVRRenderer* renderer);

            void Init(int width, int height);
            void ResizeLeft(int width, int height);
            void ResizeRight(int width, int height);

            void SetLeftProjection(OVR::Matrix4f projection);
            void SetLeftView(OVR::Matrix4f view);

            void SetRightProjection(OVR::Matrix4f projection);
            void SetRightView(OVR::Matrix4f view);

            unsigned int* Render(std::vector<RenderCommand> cmds);
            unsigned int RenderLeft(std::vector<RenderCommand> cmds, OVR::Matrix4f projection, OVR::Matrix4f view);
            unsigned int RenderRight(std::vector<RenderCommand> cmds, OVR::Matrix4f projection, OVR::Matrix4f view);
            void Render(RenderCommand cmd, OVR::Matrix4f projection, OVR::Matrix4f view);

            float yaw;
            OVR::Vector3f originPos;

            ovrFovPort leftFov;
            OVR::Vector3f leftPos;
            OVR::Quatf leftRot;

            ovrFovPort rightFov;
            OVR::Vector3f rightPos;
            OVR::Quatf rightRot;

            OVR::Matrix4f leftView;
            OVR::Matrix4f leftProj;

            OVR::Matrix4f rightView;
            OVR::Matrix4f rightProj;

            Framebuffer leftFBO, rightFBO;

        protected:




//            Matrices leftEyeMatrices;
//            Matrices rightEyeMatrices;
            OVRRenderer* renderer;
        };

    }

}

#endif //OMICRONRENDER_OVRCMDPROCESSOR_HPP
