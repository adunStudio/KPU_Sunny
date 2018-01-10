//
// Created by adunstudio on 2018-01-08.
//

#include "Timer.h"

namespace sunny
{
    namespace utils
    {
        Timer::Timer()
        {
            Reset();
        }

        // 현재 시간을 갱신한다.
        void Timer::Reset()
        {
            // now() 함수를 통해 현재 시간을 구한다.
            // now() 함수가 반환하는 기본 단위는 정밀도가 가장 높은 nano seconds 단위다.
            m_start = clock::now();
        }

        // 경과된 시간을 second 단위로 반환한다. (소수점으로 표현)
        float Timer::Elapsed()
        {
            // 나노 초 단위가 아닌 시간 단위로 표현할 때엔 duraction_cast를 통해 명시적 캐스팅을 해주어야 한다.
            return duration_cast<fs>(clock::now() - m_start).count() / 1000.0f;
        }

        // 경과된 시간을 milli second 단위로 반환한다.
        float Timer::ElapsedMillis()
        {
            return Elapsed()  * 1000.0f;
        }
    }
}