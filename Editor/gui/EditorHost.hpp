//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_EDITORHOST_HPP
#define OMICRONRENDER_EDITORHOST_HPP

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "RenderHost.h"
#include "../render/QtRenderer.hpp"

class EditorHost : public QMainWindow {
public:
    EditorHost(QWidget* parent = nullptr);
    void UpdateImageData();

    QtRenderer* renderer;
    RenderHost* renderHost;
    QImage* image;
    QGraphicsScene* scene;
};


#endif //OMICRONRENDER_EDITORHOST_HPP
