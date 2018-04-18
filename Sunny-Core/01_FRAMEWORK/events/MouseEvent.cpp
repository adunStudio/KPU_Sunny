//
// Created by adunstudio on 2018-01-13.
//

#include "MouseEvent.h"

namespace sunny
{
    namespace events {
        MouseButtonEvent::MouseButtonEvent(int button, float x, float y, Event::Type type)
        : Event(type), m_button(button), m_position(maths::vec2(x, y))
        {
        }

        MousePressedEvent::MousePressedEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y, MousePressedEvent::GetStaticType())
        {
        }

        std::string MousePressedEvent::ToString() const
        {
            char buffer[256];
            sprintf(buffer, "MouseReleasedEvent: (%d, %f,%f)", GetButton(), GetX(), GetY());
            return std::string(buffer);
        }

        MouseReleasedEvent::MouseReleasedEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y, MouseReleasedEvent::GetStaticType())
        {
        }

        MouseMovedEvent::MouseMovedEvent(float x, float y, bool dragged)
        : Event(MouseMovedEvent::GetStaticType()), m_position(maths::vec2(x, y)), m_dragged(dragged)
        {
        }
    }
}