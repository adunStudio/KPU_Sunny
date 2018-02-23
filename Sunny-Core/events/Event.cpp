//
// Created by adunstudio on 2018-01-12.
//

#include "Event.h"

namespace sunny
{
    namespace events
    {
        Event::Event(Type type)
        : m_type(type), m_handled(false)
        {

        }

        std::string Event::ToString() const
        {
            return "Event: ";
        }

        std::string Event::TypeToString(Type type)
        {
            switch (type)
            {
                case Type::KEY_PRESSED:
                    return "KEY_PRESSED";
                case Type::KEY_RELEASED:
                    return "KEY_RELEASED";
                case Type::MOUSE_PRESSED:
                    return "MOUSE_PRESSED";
                case Type::MOUSE_RELEASED:
                    return "MOUSE_RELEASED";
                case Type::MOUSE_MOVED:
                    return "MOUSE_MOVED";
            }
            return "INVALID";
        }
    }
}