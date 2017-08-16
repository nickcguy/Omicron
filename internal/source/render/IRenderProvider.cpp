//
// Created by Guy on 06/08/2017.
//

#include <render/IRenderProvider.hpp>

namespace Omicron {

    std::vector<RenderCommand> IRenderProvider::Renderables() {
        std::vector<RenderCommand> cmds;
        Renderables(cmds);
        return cmds;
    }

    std::vector<Light> IRenderProvider::Lights() {
        std::vector<Light> lights;
        Lights(lights);
        return lights;
    }

}