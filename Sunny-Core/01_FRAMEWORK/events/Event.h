//
// Created by adunstudio on 2018-01-12.
//

#pragma once

#include "../include.h"

namespace sunny
{
    namespace events
    {
        class Event
        {
        private:
            friend class EventDispatcher;

        public:
            enum class Type
            {
                SUNNY_KEY_PRESSED    = BIT(0),
				SUNNY_KEY_RELEASED   = BIT(1),

				SUNNY_MOUSE_PRESSED  = BIT(2),
				SUNNY_MOUSE_RELEASED = BIT(3),
				SUNNY_MOUSE_MOVED    = BIT(4),

				SUNNY_WINDOW_RESIZE  = BIT(5)
            };

        protected:
            bool m_handled;
            Type m_type;

        protected:
            Event(Type type);

        public:
            inline Type  GetType() const { return m_type; }
            inline bool isHandled() const { return m_handled; }

            virtual std::string ToString() const;

            static std::string TypeToString(Type type);
        };
    }
}