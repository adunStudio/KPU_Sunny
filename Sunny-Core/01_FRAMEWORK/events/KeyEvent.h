//
// Created by adunstudio on 2018-01-13.
//

#pragma once

#include "Event.h"

namespace sunny
{
    namespace events
    {
        class KeyEvent : public Event
        {
        protected:
            int m_keyCode;
            int m_count;

        public:
            KeyEvent(int keyCode, Event::Type type);

            inline int GetKeyCode() const { return m_keyCode; }
            inline static int GetStaticType() { return static_cast<int>(Event::Type::SUNNY_KEY_PRESSED) | static_cast<int>(Event::Type::SUNNY_KEY_RELEASED); }
        };

        class KeyPressedEvent : public KeyEvent
        {
        private:
            int m_repeat;
            int m_modifiers;

        public:
            KeyPressedEvent(int button, int repeat, int modifiers);

            inline int GetRepeat()    const { return m_repeat;   }
            inline int GetModifiers() const { return m_modifiers;}
            inline bool isModifier(int modifier) const { return static_cast<bool>(m_modifiers & modifier); }

            inline static Event::Type GetStaticType() { return Event::Type::SUNNY_KEY_PRESSED; }
        };

        class KeyReleasedEvent : public KeyEvent
        {
        public:
            KeyReleasedEvent(int button);

            inline static Event::Type GetStaticType() { return Event::Type::SUNNY_KEY_RELEASED; }
        };
    }
}