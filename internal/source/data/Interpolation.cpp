//
// Created by Guy on 17/08/2017.
//

#include <data/Interpolation.hpp>
#include <cmath>


namespace Omicron {

    LambdaInterpolation::LambdaInterpolation(float (* interp)(float)) : interp(interp) {}

    float LambdaInterpolation::Apply(float alpha) {
        return interp(alpha);
    }

    #if INCLUDE_SAMPLE_INTERPS

    namespace Interpolations {

        Interpolation* Linear() {
            static Interpolation* linear = new LambdaInterpolation([](float a) { return a; });
            return linear;
        }

        Interpolation* Smooth() {
            static Interpolation* smooth = new LambdaInterpolation([](float a) { return a * a * (3 - 2 * a); });
            return smooth;
        }

        Interpolation* Fade() {
            static Interpolation* fade = new LambdaInterpolation([](float a) { return a * a * a * (a * (a * 6 - 15) + 10); });
            return fade;
        }

        Interpolation* Sine() {
            static Interpolation* sine = new LambdaInterpolation([](float a) { return static_cast<float>((1.f - std::cos(a * M_PI)) / 2.f); });
            return sine;
        }

        Interpolation* SineIn() {
            static Interpolation* sineIn = new LambdaInterpolation([](float a) { return static_cast<float>(1.f - std::cos(a * M_PI / 2)); });
            return sineIn;
        }

        Interpolation* SineOut() {
            static Interpolation* sineOut = new LambdaInterpolation([](float a) { return static_cast<float>(std::sin(a * M_PI / 2)); });
            return sineOut;
        }

        Interpolation* Circle() {
            static Interpolation* circle = new LambdaInterpolation([](float a) {
                if(a <= 0.5f) {
                    a *= 2;
                    return (1 - std::sqrt(1 - a * a)) / 2;
                }
                a--;
                a *= 2;
                return (std::sqrt(1 - a * a) + 1) / 2;
            });
            return circle;
        }

        Interpolation* CircleIn() {
            static Interpolation* circleIn = new LambdaInterpolation([](float a) { return 1.f - std::sqrt(1 - a * a); });
            return circleIn;
        }

        Interpolation* CircleOut() {
            static Interpolation* circleOut = new LambdaInterpolation([](float a) {
                a--;
                return std::sqrt(1 - a * a);
            });
            return circleOut;
        }

    }

    #endif //INCLUDE_SAMPLE_INTERPS

}