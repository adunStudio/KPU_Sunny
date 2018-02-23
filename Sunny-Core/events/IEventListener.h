//
// Created by adunstudio on 2018-01-13.
//

#pragma once

#include "Event.h"

namespace sunny
{
    namespace events
    {
        class IEventListener
        {
        public:
            virtual void OnEvent(Event& event) = 0;
        };
    }
}