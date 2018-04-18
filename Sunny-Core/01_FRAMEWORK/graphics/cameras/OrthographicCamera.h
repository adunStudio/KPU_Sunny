#pragma once

#include "Camera.h"

namespace sunny
{
	namespace graphics
	{
		class OrthographicCamera : public Camera
		{
		public:
			OrthographicCamera(const maths::mat4& projectionMatrix);
		};
	}
}