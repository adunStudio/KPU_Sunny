//
// Created by adunstudio on 2018-01-08.
//

#include <chrono>
#include "../include.h"

namespace sunny
{
    namespace utils
    {
        using namespace std::chrono;

        typedef high_resolution_clock     clock;
        typedef duration<float, std::milli> fs;

        class Timer
        {
        private:
            time_point<clock> m_start;

        public:
            Timer();

            // 현재 시간을 갱신한다.
            void Reset();

            // 경과된 시간을 second 단위로 반환한다. (소수점으로 표현)
            float Elapsed();

            // 경과된 시간을 milli second 단위로 반환한다.
            float ElapsedMillis();
        };
    }
}