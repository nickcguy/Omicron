//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_QTRENDERER_HPP
#define OMICRONRENDER_QTRENDERER_HPP

#include <render/newRender/FlatRenderer.hpp>

class QtRenderer : public Omicron::FlatRenderer {
public:
    QtRenderer(Omicron::OpenGLContext* context);

    virtual void Init() override;

    virtual void BlitToScreen(int targetFBO) override;

    unsigned int fboId;
    unsigned int texId;
    std::vector<unsigned char> imageData;

};


#endif //OMICRONRENDER_QTRENDERER_HPP
