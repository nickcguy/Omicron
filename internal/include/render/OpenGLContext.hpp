//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OPENGLCONTEXT_HPP
#define OMICRONRENDER_OPENGLCONTEXT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <render/camera/Camera.hpp>
#include <render/camera/RollCamera.hpp>
#include <render/ovr/OVRRenderer.hpp>
#include <engine/input/InputProvider.hpp>
#include <render/newRender/BaseRenderer.hpp>
#include "OpenGLRenderer.hpp"

#ifndef MAX_KEYS
#define MAX_KEYS 1024
#endif

namespace Omicron {

    class IRenderProvider;
    class OmicronEngine;

    class OpenGLContext {
    public:

        explicit OpenGLContext(GLFWwindow* window, IRenderProvider* renderProvider);
        virtual void Init();
        void SetActiveContext();
        void Loop(OmicronEngine* engine = nullptr);
        void Terminate();
        void ProcessKeys(float delta);
        virtual void SpawnRenderer();
        virtual void SpawnCamera();

        virtual void Render(float delta);

        Camera* GetCamera();

        GLFWwindow* GetWindow() const;
        BaseRenderer* renderer = nullptr;
        InputProvider* inputProvider=  nullptr;

        size_t GetWidth();
        size_t GetHeight();

    protected:
        size_t windowWidth;
        size_t windowHeight;
        GLFWwindow* window = nullptr;
        IRenderProvider* renderProvider = nullptr;
        Camera* camera;
        bool firstMouse = true;
        double lastX = 0;
        double lastY = 0;
        bool keys[MAX_KEYS];

        bool terminated = false;

        virtual void Resize(int width, int height);

    };
}

#endif //OMICRONRENDER_OPENGLCONTEXT_HPP
