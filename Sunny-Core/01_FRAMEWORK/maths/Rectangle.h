//
// Created by adunstudio on 2018-01-07.
//

#pragma once

#include "../include.h"
#include "vec2.h"
#include "vec3.h"

namespace sunny
{
	enum PIVOT { PIVOT_LEFT, PIVOT_CENTER };

    namespace maths
    {
        struct Rectangle
        {
			PIVOT pivot;

            union
            {
                vec2 position;
                struct
                {
                    float x;
                    float y;
                };
            };

            union
            {
                vec2 size;
                struct
                {
                    float width;
                    float height;
                };
            };



            Rectangle(PIVOT pivot = PIVOT_LEFT);
            Rectangle(const vec2& position, const vec2& size, PIVOT pivot = PIVOT_LEFT);
            Rectangle(float x, float y, float width, float height, PIVOT pivot = PIVOT_LEFT);

            bool Intersects(const Rectangle& other) const;
            bool Contains(const vec2& point) const;
            bool Contains(const vec3& point) const;

			inline vec2 GetMinimumBound() const { if (pivot == PIVOT_LEFT) return position; else return position - size; }
            inline vec2 GetMaximumBound() const { return position + size; }

            bool operator==(const Rectangle& other) const;
            bool operator!=(const Rectangle& other) const;

            bool operator<(const Rectangle& other) const;
            bool operator>(const Rectangle& other) const;

            friend std::ostream& operator<<(std::ostream& stream, const Rectangle& Rectangle);
        };
    }
}



