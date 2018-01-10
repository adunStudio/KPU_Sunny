//
// Created by adunstudio on 2018-01-11.
//

#pragma once

namespace sunny
{
    namespace utils
    {
        struct Timestep
        {
        private:
            float m_timeStep;
            float m_lastTime;

        public:
            inline Timestep(float initialTime)
            : m_timeStep(0.0f), m_lastTime(initialTime)
            {
            }

            inline void Update(float currentTime)
            {
                m_timeStep = currentTime - m_lastTime;
                m_lastTime = currentTime;
            }

            inline float GetMillis()  const { return m_timeStep; }
            inline float GetSeconds() const { return m_timeStep * 0.001f; }
        };
    }
}
