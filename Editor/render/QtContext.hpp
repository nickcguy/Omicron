//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_QTCONTEXT_HPP
#define OMICRONRENDER_QTCONTEXT_HPP

#include <render/OpenGLContext.hpp>

class QtContext : public Omicron::OpenGLContext {
public:
    QtContext(GLFWwindow* window, Omicron::IRenderProvider* renderProvider);

    virtual void Loop(Omicron::OmicronEngine* engine) override;

    virtual void SpawnRenderer() override;
};


#endif //OMICRONRENDER_QTCONTEXT_HPP
