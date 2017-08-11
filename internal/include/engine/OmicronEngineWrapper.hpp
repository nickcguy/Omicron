//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENGINEWRAPPER_HPP
#define OMICRONRENDER_OMICRONENGINEWRAPPER_HPP

#include <thread>
#include "OmicronEngine.hpp"

namespace Omicron {

    class OmicronEngineWrapper {
    public:
        explicit OmicronEngineWrapper(std::string name, int targetIps);
        void Stop();
        void SetChild(OmicronEngine* child);
        OmicronEngine* GetChild();
        void SetTargetIPS(int targetIps);

        static OmicronEngineWrapper* GetEngineWrapper(std::string name);

    protected:
        std::string name;
        std::thread engineThread;
        OmicronEngine* child;
    };

}

#endif //OMICRONRENDER_OMICRONENGINEWRAPPER_HPP
