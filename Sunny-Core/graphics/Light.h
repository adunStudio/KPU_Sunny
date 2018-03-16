#pragma once

#include "../maths/maths.h"

namespace sunny
{
	namespace graphics
	{
		__declspec(align(16)) struct Light
		{

			maths::vec4 color;            
			maths::vec3 position; 
			float p0;
			maths::vec3 ambientDown;               // ambientDown
			float p1;
			maths::vec3 ambientRange;             // ambient
			float intensity;             

			Light(const maths::vec3& ambientDown, float intensity = 1.0f, const maths::vec4& color = maths::vec4(1.0f));
		};
	}
}