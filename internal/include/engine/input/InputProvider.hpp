//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_INPUTPROVIDER_HPP
#define OMICRONRENDER_INPUTPROVIDER_HPP

namespace Omicron {

    enum InputProviderType {
        NONE,
        KBM,
        OVR,
        InputProviderType_LENGTH
    };

    class InputProvider {
    public:
        inline virtual InputProviderType GetType() {
            return NONE;
        }

    };

}

#endif //OMICRONRENDER_INPUTPROVIDER_HPP
