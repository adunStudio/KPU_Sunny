//
// Created by adunstudio on 2018-01-12.
//

#pragma once

#include "../sunny.h"

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
                KEY_PRESSED    = BIT(0),
                KEY_RELEASED   = BIT(1),

                MOUSE_PRESSED  = BIT(2),
                MOUSE_RELEASED = BIT(3),
                MOUSE_MOVED     = BIT(4),

                WINDOW_RESIZE = BIT(5)
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