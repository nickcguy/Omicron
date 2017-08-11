//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_ISCRIPTABLE_HPP
#define OMICRONRENDER_ISCRIPTABLE_HPP

#include <lua.hpp>
#include <selene/State.h>
#include "ScriptHost.hpp"

#define REGISTER_SCRIPTABLE(cls) namespace { ScriptableAdapter adapter_##cls = ScriptableAdapter(new cls); }

namespace Omicron {

    class IScriptable {
    public:
        virtual void Register(const sel::State& state)=0;
        virtual const char* Name()=0;
    };

    struct ScriptableAdapter {
        explicit ScriptableAdapter(IScriptable* s);
    };

}


#endif //OMICRONRENDER_ISCRIPTABLE_HPP
