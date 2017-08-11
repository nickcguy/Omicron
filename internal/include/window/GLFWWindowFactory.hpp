//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_GLFWWINDOWFACTORY_HPP
#define OMICRONRENDER_GLFWWINDOWFACTORY_HPP

#include <GLFW/glfw3.h>

namespace Omicron {

    class GLFWWindowFactory {
    public:
        GLFWWindowFactory();

        GLFWWindowFactory& SetResizable(bool resizable);

        GLFWWindowFactory& SetAPI(int api);

        GLFWWindowFactory& SetCaptureCursor(bool captureCursor);

        GLFWWindowFactory& SetWidth(int width);

        GLFWWindowFactory& SetHeight(int height);

        GLFWWindowFactory& SetSize(int width, int height);

        GLFWWindowFactory& SetTitle(const char* title);

        GLFWWindowFactory& SetMonitor(GLFWmonitor* monitor);

        GLFWWindowFactory& SetShare(GLFWwindow* share);

        GLFWWindowFactory& SetVersionMajor(int major);

        GLFWWindowFactory& SetVersionMinor(int minor);

        GLFWWindowFactory& SetVersion(int major, int minor);

        GLFWWindowFactory& SetProfile(unsigned int profile);

        GLFWWindowFactory& SetUserPointer(void* ptr);



        GLFWwindow* Build();

    protected:
        int width = 800;
        int height = 600;
        int versionMajor = 3;
        int versionMinor = 3;
        unsigned int profile = GLFW_OPENGL_CORE_PROFILE;
        int api = GLFW_OPENGL_API;
        const char* title = "No title";
        GLFWmonitor* monitor = nullptr;
        GLFWwindow* share = nullptr;

        bool captureCursor = false;
        bool resizable = false;

        void* ptr = nullptr;
    };
};

#endif //OMICRONRENDER_GLFWWINDOWFACTORY_HPP
