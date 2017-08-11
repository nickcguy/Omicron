//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_MATERIALSYSTEM_HPP
#define OMICRONRENDER_MATERIALSYSTEM_HPP

#include "OmicronSystem.hpp"

namespace Omicron {

    class MaterialSystem : public OmicronSystem {
    public:
        virtual void Update(float delta) override;

        virtual std::string Name() override;
    };

}

#endif //OMICRONRENDER_MATERIALSYSTEM_HPP
