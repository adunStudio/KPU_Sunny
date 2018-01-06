//
// Created by adunstudio on 2018-01-07.
//

#pragma once

#include "../sunny.h"

namespace sunny
{
    namespace maths
    {
        const float SUNNY_PI = 3.14159265358f;

        // 각도 단위로 지정된 인자를 라디안 단위로 환산하여 반환한다.
        inline float toRadians(float degree)
        {
            return static_cast<float>(degree * (SUNNY_PI / 180.0f));
        };

        // 라디안 단위로 지정된 인자를 각도로 환산하여 반환한다.
        inline float toDegrees(float radians)
        {
            return static_cast<float>(radians * (180.0f / SUNNY_PI));
        }

        // 지정된 인자 값의 부호를 반환한다. 양수이면 1을, 음수이면 -1을, 0이면 0을 반환한다
        inline int sign(float value)
        {
            //  1:  true  -  false =  1
            //  0: false  -  false =  0
            // -1: false  -   true = -1
            return (value > 0) - (value < 0);
        }

        // 인자의 사인(sine)값을 반환하며, 인자 angle은 라디안 값이어야 한다.
        inline float sin(float angle)
        {   // 높이 / 빗변
            return ::sin(angle);
        }

        // 인자의 코사인(cosine)값을 반환하며, 인자 angle은 라디안 값이어야 한다.
        inline float cos(float angle)
        {   // 밑변 / 빗변
            return ::cos(angle);
        }

        // 인자의 탄젠트(tangent)값을 반환하며, 인자 angle은 라디안 값이어야 한다.
        inline float tan(float angle)
        {   // 높이 / 밑변
            return ::tan(angle);
        }

        // 인자값의 제곱근을 반환한다.
        inline float sqrt(float value)
        {
            return ::sqrt(value);
        }

        // 인자값 제곱근의 역수를 반환한다.
        inline float rsqrt(float value)
        {
            return 1.0f / ::sqrt(value);
        }

        // 인자의 아크사인(arcsine)값을 반환한다.
        inline float asin(float value)
        {
            return ::asin(value);
        }

        // 인자의 아크코사인(arccosine)값을 반환한다.
        inline float acon(float value)
        {
            return ::acos(value);
        }

        // 인자의 아크탄젠트(arctangent)값을 반환한다.
        inline float atan(float value)
        {
            return ::atan(value);
        }

        // 인자의 아크탄젠트(arctangent)값을 반환한다.
        inline float atan2(float y, float x)
        {
            return ::atan2(y, x);
        }

        // 최소값 제한 함수
        inline float _min(float value, float minimum)
        {
            return (value < minimum) ? minimum : value;
        }

        // 최대값 제한 함수
        inline float _max(float value, float maximum)
        {
            return (value > maximum) ? maximum : value;
        }

        // 최소값과 최대값 제한 함수
        inline float clamp(float value, float minimum, float maximum)
        {
            return (value > minimum) ? (value < maximum) ? value : maximum : minimum;
        }
    }
}
