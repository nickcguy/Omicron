//
// Created by Guy on 19/07/2017.
//

#include <window/GLFWWindowFactory.hpp>

namespace Omicron {

    GLFWWindowFactory::GLFWWindowFactory() = default;

    GLFWWindowFactory& GLFWWindowFactory::SetResizable(bool resizable) {
        this->resizable = resizable;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetAPI(int api) {
        this->api = api;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetCaptureCursor(bool captureCursor) {
        this->captureCursor = captureCursor;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetWidth(int width) {
        this->width = width;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetHeight(int height) {
        this->height = height;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetSize(int width, int height) {
        return SetWidth(width).SetHeight(height);
    }

    GLFWWindowFactory& GLFWWindowFactory::SetTitle(const char* title) {
        this->title = title;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetMonitor(GLFWmonitor* monitor) {
        this->monitor = monitor;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetShare(GLFWwindow* share) {
        this->share = share;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetVersionMajor(int major) {
        this->versionMajor = major;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetVersionMinor(int minor) {
        this->versionMinor = minor;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetVersion(int major, int minor) {
        return SetVersionMajor(major).SetVersionMinor(minor);
    }

    GLFWWindowFactory& GLFWWindowFactory::SetProfile(unsigned int profile) {
        this->profile = profile;
        return *this;
    }

    GLFWWindowFactory& GLFWWindowFactory::SetUserPointer(void* ptr) {
        this->ptr = ptr;
        return *this;
    }

    GLFWwindow* GLFWWindowFactory::Build() {

        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, api);
        glfwWindowHint(GLFW_RESIZABLE, resizable ? 1 : 0);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, share);

        if(ptr != nullptr)
            glfwSetWindowUserPointer(window, ptr);

        if(captureCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
    }

};