//
// Created by adunstudio on 2018-01-13.
//

#include "KeyEvent.h"

namespace sunny
{
    namespace events
    {
        KeyEvent::KeyEvent(int keyCode, Event::Type type)
        : Event(type), m_keyCode(keyCode)
        {}

        KeyPressedEvent::KeyPressedEvent(int button, int repeat, int modifiers)
        : KeyEvent(button, KeyPressedEvent::GetStaticType()), m_repeat(repeat), m_modifiers(modifiers)
        {}

        KeyReleasedEvent::KeyReleasedEvent(int button)
        : KeyEvent(button, KeyReleasedEvent::GetStaticType())
        {}
    }
}
