//
// Created by Guy on 09/08/2017.
//

#ifndef OMICRONRENDER_SYSTEMADAPTER_HPP
#define OMICRONRENDER_SYSTEMADAPTER_HPP

#include <script/IScriptable.hpp>
#include <string>

namespace Omicron {

    struct SystemAccessor {
        void PrintMessage(std::string s);
        float DegToRad(float deg);
        float RadToDeg(float rad);
    };

    class SystemAdapter : public IScriptable {
    public:
        virtual void Register(const sel::State& state) override;

        virtual const char* Name() override;
    };

    REGISTER_SCRIPTABLE(SystemAdapter)

}

#endif //OMICRONRENDER_SYSTEMADAPTER_HPP
