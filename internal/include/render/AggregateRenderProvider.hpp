//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_AGGREGATERENDERPROVIDER_HPP
#define OMICRONRENDER_AGGREGATERENDERPROVIDER_HPP

#include "IRenderProvider.hpp"

namespace Omicron {

    class AggregateRenderProvider : public IRenderProvider {
    public:
        void AddProvider(IRenderProvider* providerPtr);
        void RemoveProvider(IRenderProvider* providerPtr);

        virtual void Renderables(std::vector<RenderCommand>& vector) override;

        virtual void Lights(std::vector<Light>& vector) override;

    protected:
        std::vector<IRenderProvider*> providerPtrs;
    };

};

#endif //OMICRONRENDER_AGGREGATERENDERPROVIDER_HPP
