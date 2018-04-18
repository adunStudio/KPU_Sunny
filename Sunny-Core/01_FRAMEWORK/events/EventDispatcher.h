//
// Created by adunstudio on 2018-01-12.
//

#pragma once

#include "../include.h"
#include "Event.h"
#include <functional>

namespace sunny
{
    namespace events
    {
        class EventDispatcher
        {
        private:
            Event& m_event;

        public:
            EventDispatcher(Event& event) : m_event(event)
            {
            }

            template<typename T>
            void Dispatch(std::function<bool(T&)> func)
            {
                // std::function<리턴타입(입력 파라미터들)> 변수
                // void print(int a, int b);
                // ex) std::function<void(int, int)> fp = print;
                if(static_cast<int>(m_event.GetType()) & static_cast<int>(T::GetStaticType()))
                    m_event.m_handled = func(*(T*)&m_event);
            }
        };
    }
}