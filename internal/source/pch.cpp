//
// Created by Guy on 10/08/2017.
//

#include <pch.hpp>

namespace Omicron {

    namespace {
        unsigned int drawCalls = 0;
    }

    int GetDrawCalls() {
        return drawCalls;
    }

    void IncrementDrawCalls() {
        drawCalls++;
    }

    void ResetDrawCalls() {
        drawCalls = 0;
    }

}