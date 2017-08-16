//
// Created by Guy on 15/08/2017.
//

#include <QtCore/QByteRef>
#include <QGraphicsScene>
#include <window/GLFWWindowFactory.hpp>
#include <engine/OmicronEngineWrapper.hpp>
#include <render/AggregateRenderProvider.hpp>
#include "EditorHost.hpp"
#include "../render/QtContext.hpp"
#include "../render/QtRenderer.hpp"
#include <QLabel>
#include <QTimer>

EditorHost::EditorHost(QWidget* parent) : QMainWindow(parent) {
    renderHost = new RenderHost(this);

    Omicron::GLFWWindowFactory factory;
    GLFWwindow* window = factory.SetVersion(4, 5)
                                .SetTitle("Offscreen Omicron Renderer")
                                .SetProfile(0x00032001)
                                .SetResizable(false)
                                .SetVisible(false)
                                .Build();

    Omicron::OmicronEngineWrapper engineWrapper("CoreEngine", 30);
    Omicron::OmicronEngine* engine = engineWrapper.GetChild();
    Omicron::AggregateRenderProvider renderProvider = {};
    renderProvider.AddProvider(engine);

    auto context = new QtContext(window, &renderProvider);
    context->Init();

    renderer = reinterpret_cast<QtRenderer*>(context->renderer);


    image = new QImage();
    scene = new QGraphicsScene(this);

//renderHost->glWidget->SetTextureId(renderer->texId);
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &EditorHost::UpdateImageData);
    timer->start(20);
}

void EditorHost::UpdateImageData() {
    QByteArray arr;
    for(auto d : renderer->imageData)
        arr.push_back(d);
    image->loadFromData(arr);
    scene->addPixmap(QPixmap::fromImage(*image));
}
