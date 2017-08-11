//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_INPUTPROVIDER_HPP
#define OMICRONRENDER_INPUTPROVIDER_HPP

namespace Omicron {

    enum InputProviderType {
        KBM,
        OVR,
        InputProviderType_LENGTH
    };

    class InputProvider {
    public:
        virtual InputProviderType GetType()=0;

    };

}

#endif //OMICRONRENDER_INPUTPROVIDER_HPP
