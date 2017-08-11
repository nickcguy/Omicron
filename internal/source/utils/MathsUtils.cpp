//
// Created by Guy on 06/08/2017.
//

#include <utils/MathsUtils.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Omicron {

    namespace Utils {

        namespace {
            unsigned int gSeed;
        }

        void fastSrand(unsigned int seed) {
            gSeed = seed;
        }

        int fastRand() {
            gSeed = 214013 * gSeed + 2531011;
            return (gSeed >> 16) & 0x7FFF;
        }

        int fastRand_Range(int min, int max) {
            return (fastRand() % max) + min;

        }

        glm::vec3 ToEulerianAngle(const glm::quat& q) {

            float roll = 0;
            float pitch = 0;
            float yaw = 0;

            float ysqr = q.y * q.y;
            float t0 = 2.f * (q.w * q.x + q.y * q.z);
            float t1 = 1.f - 2.f * (q.x * q.x + ysqr);
            roll = std::atan2(t0, t1);

            float t2 = 2.f * (q.w * q.y - q.z * q.x);
            t2 = glm::clamp(t2, -1.f, 1.f);
            pitch = std::asin(t2);

            float t3 = 2.f * (q.w * q.z + q.x * q.y);
            float t4 = 1.f - 2.f * (ysqr + q.z * q.z);
            yaw = std::atan2(t3, t4);

            return {roll, pitch, yaw};
        }

    }

}