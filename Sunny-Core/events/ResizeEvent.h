//
// Created by adunstudio on 2018-01-16.
//

#include "Event.h"
#include "../maths/tvec2.h"

namespace sunny
{
    namespace events
    {
        class ResizeEvent : public Event
        {
        protected:
            maths::tvec2<int> m_size;

        public:
            ResizeEvent(int width, int height);

            inline const maths::tvec2<int>& GetSize() const { return m_size; }
            inline const int GetWidth() const { return m_size.x; }
            inline const int GetHeight() const { return m_size.y; }

        public:
            inline static Type GetStaticType() { return Event::Type::WINDOW_RESIZE; }
        };
    }
}