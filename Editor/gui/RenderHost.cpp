//
// Created by Guy on 15/08/2017.
//

#include "RenderHost.h"

RenderHost::RenderHost(QWidget* parent) : QWidget(parent) {
    glWidget = new GLWidget(this);
}
