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
                case Type::SUNNY_KEY_PRESSED:
                    return "SUNNY_KEY_PRESSED";
                case Type::SUNNY_KEY_RELEASED:
                    return "SUNNY_KEY_RELEASED";
                case Type::SUNNY_MOUSE_PRESSED:
                    return "SUNNY_MOUSE_PRESSED";
                case Type::SUNNY_MOUSE_RELEASED:
                    return "SUNNY_MOUSE_RELEASED";
                case Type::SUNNY_MOUSE_MOVED:
                    return "SUNNY_MOUSE_MOVED";
				case Type::SUNNY_WINDOW_RESIZE:
					return "SUNNY_WINDOW_RESIZE";
				case Type::SUNNY_SERVER_PACKET:
					return "SUNNY_SERVER_PACKET";
				case Type::SUNNY_SERVER_CLOSE:
					return "SUNNY_SERVER_CLOSE";
            }
            return "SUNNY_INVALID";
        }
    }
}