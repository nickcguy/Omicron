//
// Created by Guy on 10/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENGINEADAPTER_HPP
#define OMICRONRENDER_OMICRONENGINEADAPTER_HPP

#include <vector>
#include <script/IScriptable.hpp>
#include "OmicronEntityAccessor.hpp"

namespace Omicron {

    class OmicronEngine;

    struct OmicronEngineAccessor {
//        explicit OmicronEngineAccessor();
        OmicronEngineAccessor(std::string engineName);
//        explicit OmicronEngineAccessor(OmicronEngine* engine);

        std::vector<OmicronEntityAccessor> GetTagged(std::string tag);
        int GetTaggedCount(std::string tag);
        OmicronEntityAccessor GetTaggedEntity(std::string tag, int id);

        int GetEntityCount();

        OmicronEngine* GetEngine();
        std::string engineName;
    };

    class OmicronEngineAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(OmicronEngineAdapter)

}

#endif //OMICRONRENDER_OMICRONENGINEADAPTER_HPP
