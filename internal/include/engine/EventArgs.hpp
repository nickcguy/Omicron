//
// Created by Guy on 19/08/2017.
//

#ifndef OMICRONRENDER_EVENTARGS_HPP
#define OMICRONRENDER_EVENTARGS_HPP

#include <string>
#include <utility>
#include <vector>

namespace Omicron {

    enum ActivePrimitive {
        PRIMITIVE_NONE,
        PRIMITIVE_INT,
        PRIMITIVE_BOOL,
        PRIMITIVE_FLOAT,
        ActivePrimitive_LENGTH
    };

    struct PrimitiveVariant {
        int integer;
        bool boolean;
        float floating;

        ActivePrimitive type = PRIMITIVE_NONE;


        PrimitiveVariant operator = (int i) {
            integer = i;
            type = PRIMITIVE_INT;
        }

        PrimitiveVariant operator = (bool b) {
            boolean = b;
            type = PRIMITIVE_BOOL;
        }

        PrimitiveVariant operator = (float f) {
            floating = f;
            type = PRIMITIVE_FLOAT;
        }

        explicit PrimitiveVariant() {}

        explicit PrimitiveVariant(int   i) : integer(i),  type(PRIMITIVE_INT)   {}
        explicit PrimitiveVariant(bool  b) : boolean(b),  type(PRIMITIVE_BOOL)  {}
        explicit PrimitiveVariant(float f) : floating(f), type(PRIMITIVE_FLOAT) {}

    };

    struct EngineEvent {
        std::string eventName;
        PrimitiveVariant data;

        EngineEvent(std::string eventName, PrimitiveVariant data)
            : eventName(std::move(eventName)), data(data) {}

    };

    struct EventArgs {
        std::string eventName;
        PrimitiveVariant data;
    };

}

#endif //OMICRONRENDER_EVENTARGS_HPP
