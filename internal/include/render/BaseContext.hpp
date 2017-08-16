//
// Created by Guy on 15/08/2017.
//

#ifndef OMICRONRENDER_BASICCONTEXT_HPP
#define OMICRONRENDER_BASICCONTEXT_HPP

#include <engine/input/InputProvider.hpp>
#include <render/newRender/BaseRenderer.hpp>
#include <render/camera/Camera.hpp>
#include "IRenderProvider.hpp"

namespace Omicron {

    template <typename Window>
    class BaseContext {
    public:
        inline explicit BaseContext(Window* window, IRenderProvider* renderProvider) : window(window), renderProvider(renderProvider) {}
        inline Window* GetWindow() const { return window; }
        inline IRenderProvider* GetRenderProvider() { return renderProvider; }

        virtual void Init()=0;
        virtual void SetActiveContext()=0;
        virtual void Loop(OmicronEngine* engine = nullptr)=0;

        virtual size_t GetWidth()=0;
        virtual size_t GetHeight()=0;

        virtual void Render(float delta)=0;

        virtual void SpawnRenderer()=0;
        virtual void SpawnCamera()=0;

        BaseRenderer* renderer = nullptr;
        InputProvider* inputProvider=  nullptr;

        virtual Camera* GetCamera()=0;

    protected:
        Window* window = nullptr;
        IRenderProvider* renderProvider = nullptr;

        virtual void Resize(int width, int height)=0;
    };

}

#endif //OMICRONRENDER_BASICCONTEXT_HPP
