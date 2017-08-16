//
// Created by Guy on 15/08/2017.
//

#include "QtContext.hpp"
#include "QtRenderer.hpp"

QtContext::QtContext(GLFWwindow* window, Omicron::IRenderProvider* renderProvider)
: OpenGLContext(window, renderProvider) {}

void QtContext::SpawnRenderer() {
    renderer = new QtRenderer(this);
}

void QtContext::Loop(Omicron::OmicronEngine* engine) {
    Render(1.f / 60.f);
}
