//
// Created by Guy on 08/08/2017.
//

#ifndef OMICRONRENDER_OMICRONMOTIONSTATE_HPP
#define OMICRONRENDER_OMICRONMOTIONSTATE_HPP

#include <engine/entity/OmicronEntity.hpp>
#include <LinearMath/btMotionState.h>

namespace Omicron {

    class OmicronMotionState : public btMotionState {
    public:
        OmicronMotionState(OmicronEntity* entity, const btTransform& initialPosition);
        virtual ~OmicronMotionState();

        void SetEntity(OmicronEntity* entity);

        virtual void getWorldTransform(btTransform& worldTrans) const override;

        virtual void setWorldTransform(const btTransform& worldTrans) override;

    protected:
        OmicronEntity* entity;
        btTransform initialPosition;

    };

}

#endif //OMICRONRENDER_OMICRONMOTIONSTATE_HPP
