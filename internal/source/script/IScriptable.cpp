//
// Created by Guy on 11/08/2017.
//

#include <script/IScriptable.hpp>

namespace Omicron {

    ScriptableAdapter::ScriptableAdapter(Omicron::IScriptable* s) {
        ScriptHost::RegisterScriptableAdapter(s);
    }

}