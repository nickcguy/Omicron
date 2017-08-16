//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OPENGLCONTEXT_HPP
#define OMICRONRENDER_OPENGLCONTEXT_HPP

#include <glad/glad.h>
#include <render/camera/Camera.hpp>
#include <render/camera/RollCamera.hpp>
#include <render/ovr/OVRRenderer.hpp>
#include <engine/input/InputProvider.hpp>
#include <render/newRender/BaseRenderer.hpp>
#include <GLFW/glfw3.h>
#include "OpenGLRenderer.hpp"
#include "BaseContext.hpp"

#ifndef MAX_KEYS
#define MAX_KEYS 1024
#endif

namespace Omicron {

    class IRenderProvider;
    class OmicronEngine;

    class OpenGLContext : public BaseContext<GLFWwindow> {
    public:

        explicit OpenGLContext(GLFWwindow* window, IRenderProvider* renderProvider);
        virtual void Init() override;
        void SetActiveContext() override;
        void Loop(OmicronEngine* engine = nullptr) override;
        void Terminate();
        void ProcessKeys(float delta);
        virtual void SpawnRenderer() override;
        virtual void SpawnCamera() override;

        virtual void Render(float delta) override;
        virtual void SetWindowShouldClose(bool shouldClose);

        virtual Camera* GetCamera() override;

        size_t GetWidth() override;
        size_t GetHeight() override;

    protected:
        size_t windowWidth;
        size_t windowHeight;
        Camera* camera;
        bool firstMouse = true;
        double lastX = 0;
        double lastY = 0;
        bool keys[MAX_KEYS];

        bool terminated = false;

        virtual void Resize(int width, int height) override;

    };
}

#endif //OMICRONRENDER_OPENGLCONTEXT_HPP
