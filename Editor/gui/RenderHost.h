//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_RENDERHOST_H
#define OMICRONRENDER_RENDERHOST_H


#include <QtWidgets/QWidget>
#include "widgets/GLWidget.hpp"

class RenderHost : public QWidget {
public:
    RenderHost(QWidget* parent = nullptr);

    GLWidget* glWidget;
};


#endif //OMICRONRENDER_RENDERHOST_H
