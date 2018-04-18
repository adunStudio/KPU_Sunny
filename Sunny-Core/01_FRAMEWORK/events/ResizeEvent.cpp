//
// Created by adunstudio on 2018-01-16.
//

#include "ResizeEvent.h"

namespace sunny
{
    namespace events
    {
        ResizeEvent::ResizeEvent(int width, int height)
        : Event(ResizeEvent::GetStaticType()), m_size(maths::tvec2<int>(width, height))
        {
        }
    }
}