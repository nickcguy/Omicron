//
// Created by Guy on 06/08/2017.
//

#include <render/AggregateRenderProvider.hpp>
#include <algorithm>

namespace Omicron {

    void AggregateRenderProvider::AddProvider(IRenderProvider* providerPtr) {
        providerPtrs.push_back(providerPtr);
    }

    void AggregateRenderProvider::RemoveProvider(IRenderProvider* providerPtr) {
        providerPtrs.erase(std::remove_if(providerPtrs.begin(), providerPtrs.end(), [providerPtr](auto ptr) -> bool {
            return ptr == providerPtr;
        }));
    }

    void AggregateRenderProvider::Renderables(std::vector<RenderCommand>& cmds) {
        for(auto providerPtr : providerPtrs)
            providerPtr->Renderables(cmds);
    }
};