//
// Created by Guy on 17/08/2017.
//

#ifndef OMICRONRENDER_INTERPOLATION_HPP
#define OMICRONRENDER_INTERPOLATION_HPP

#define INCLUDE_SAMPLE_INTERPS false

namespace Omicron {

    class Interpolation {
    public:
        virtual float Apply(float alpha)=0;

        template <typename T> inline T Apply(T start, T end, float alpha) {
            return start + (end - start) * Apply(alpha);
        }
    };

    class LambdaInterpolation : public Interpolation {
    public:
        LambdaInterpolation(float (* interp)(float));

        virtual float Apply(float alpha) override;

    protected:
        float(* interp)(float);
    };

    #if INCLUDE_SAMPLE_INTERPS

    namespace Interpolations {

        static Interpolation* Linear();
        static Interpolation* Smooth();
        static Interpolation* Fade();
        static Interpolation* Sine();
        static Interpolation* SineIn();
        static Interpolation* SineOut();
        static Interpolation* Circle();
        static Interpolation* CircleIn();
        static Interpolation* CircleOut();

    }
    #endif //INCLUDE_SAMPLE_INTERPS

}

#endif //OMICRONRENDER_INTERPOLATION_HPP
