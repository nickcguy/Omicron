#include <iostream>
#include <QApplication>
#include <engine/OmicronEngineWrapper.hpp>
#include <render/AggregateRenderProvider.hpp>
#include <render/OpenGLContext.hpp>
#include <window/GLFWWindowFactory.hpp>
#include "gui/EditorHost.hpp"
#include "gui/RenderHost.h"
#include "render/QtRenderer.hpp"

int main(int argc, char** argv) {

    QApplication a(argc, argv);
    EditorHost host;
//    host.renderHost->glWidget->SetTextureId(renderer->texId);
    host.show();

    return a.exec();
}

