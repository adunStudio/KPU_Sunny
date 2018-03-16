#include "Light.h"

namespace sunny
{
	namespace graphics
	{
		Light::Light(const maths::vec3& ambientDown, float intensity, const maths::vec4& color)
		: ambientDown(ambientDown), intensity(intensity), color(color), p0(0.0f), p1(0.0f), position(maths::vec3(1.0f, -1.0f, 0.0f)), ambientRange(maths::vec3(0.0f, 0.0f, 0.0f))
		{
		}
	}
}