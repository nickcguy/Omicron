//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_MATHSUTILS_HPP
#define OMICRONRENDER_MATHSUTILS_HPP

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <glm/glm.hpp>
#include <Extras/OVR_Math.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if INCLUDE_AVATAR
#include <OVR_Avatar.h>
#endif

using namespace std;

namespace Omicron {

    namespace Utils {

        void fastSrand(unsigned int seed);
        int fastRand();
        int fastRand_Range(int min, int max);
        glm::vec3 ToEulerianAngle(const glm::quat& q);

        inline glm::vec3 ComputeForwardVector(glm::quat quat) {
            glm::mat3 mat = glm::mat3_cast(quat);

            return mat[2];

        }

        inline glm::vec3 ConvertVec3(OVR::Vector3f vec) {
            glm::vec3 vec3;
            vec3.x = vec.x;
            vec3.y = vec.y;
            vec3.z = vec.z;
            return vec3;
        }


        inline glm::quat ConvertQuat(ovrQuatf quat) {
            glm::quat q;
            q.x = quat.x;
            q.y = quat.y;
            q.z = quat.z;
            q.w = quat.w;
            return q;
        }

        inline OVR::Quatf ConvertQuat(glm::quat quat) {
            OVR::Quatf q;
            q.x = quat.x;
            q.y = quat.y;
            q.z = quat.z;
            q.w = quat.w;
            return q;
        }

        inline glm::mat4 ConvertMat4(ovrMatrix4f mat4) {
            glm::mat4 mat;
            for(int x = 0; x < 4; x++) for(int y = 0; y < 4; y++)
                    mat[x][y] = mat4.M[x][y];
            return mat;
        }

        inline OVR::Matrix4f ConvertMat4(glm::mat4 mat4) {
            OVR::Matrix4f mat;
            for(int x = 0; x < 4; x++) for(int y = 0; y < 4; y++)
                    mat.M[x][y] = mat4[x][y];
            return mat;
        }

        #if INCLUDE_AVATAR
        inline glm::mat4 ConvertOVRAvatarTransform(ovrAvatarTransform transform) {
            glm::vec3 pos(transform.position.x, transform.position.y, transform.position.z);
            glm::quat rot(transform.orientation.w, transform.orientation.x, transform.orientation.y, transform.orientation.z);
            glm::vec3 scl(transform.scale.x, transform.scale.y, transform.scale.z);
            return glm::translate(glm::mat4(), pos) * glm::mat4_cast(rot) * glm::scale(glm::mat4(), scl);
        }
        #endif


/*! \brief Convert RGB to HSV color space

  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.

  \param fR Red component, used as input, range: [0, 1]
  \param fG Green component, used as input, range: [0, 1]
  \param fB Blue component, used as input, range: [0, 1]
  \param fH Hue component, used as output, range: [0, 360]
  \param fS Hue component, used as output, range: [0, 1]
  \param fV Hue component, used as output, range: [0, 1]

*/
        inline void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
            float fCMax = max(max(fR, fG), fB);
            float fCMin = min(min(fR, fG), fB);
            float fDelta = fCMax - fCMin;

            if(fDelta > 0) {
                if(fCMax == fR) {
                    fH = 60 * (fmod(((fG - fB) / fDelta), 6));
                } else if(fCMax == fG) {
                    fH = 60 * (((fB - fR) / fDelta) + 2);
                } else if(fCMax == fB) {
                    fH = 60 * (((fR - fG) / fDelta) + 4);
                }

                if(fCMax > 0) {
                    fS = fDelta / fCMax;
                } else {
                    fS = 0;
                }

                fV = fCMax;
            } else {
                fH = 0;
                fS = 0;
                fV = fCMax;
            }

            if(fH < 0) {
                fH = 360 + fH;
            }
        }


/*! \brief Convert HSV to RGB color space

  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.

  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]

*/
        inline void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV) {
            float fC = fV * fS; // Chroma
            float fHPrime = fmod(fH / 60.0, 6);
            float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
            float fM = fV - fC;

            if(0 <= fHPrime && fHPrime < 1) {
                fR = fC;
                fG = fX;
                fB = 0;
            } else if(1 <= fHPrime && fHPrime < 2) {
                fR = fX;
                fG = fC;
                fB = 0;
            } else if(2 <= fHPrime && fHPrime < 3) {
                fR = 0;
                fG = fC;
                fB = fX;
            } else if(3 <= fHPrime && fHPrime < 4) {
                fR = 0;
                fG = fX;
                fB = fC;
            } else if(4 <= fHPrime && fHPrime < 5) {
                fR = fX;
                fG = 0;
                fB = fC;
            } else if(5 <= fHPrime && fHPrime < 6) {
                fR = fC;
                fG = 0;
                fB = fX;
            } else {
                fR = 0;
                fG = 0;
                fB = 0;
            }

            fR += fM;
            fG += fM;
            fB += fM;
        }

    }

}

#endif //OMICRONRENDER_MATHSUTILS_HPP
