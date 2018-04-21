//
// Created by adunstudio on 2018-04-21.
//
#pragma once

#include "../include.h"
#include "vec3.h"

namespace sunny
{
	namespace maths
	{
		struct OBB
		{
			vec3 center;        // 중앙
			vec3 axis;          // 세 축의 단위 벡터
			vec3 len;           // 세 축의 길이
		
			OBB(const vec3& center, const vec3& axis, const vec3& len);

			bool Intersects(const OBB& other) const;
		};
	}
}