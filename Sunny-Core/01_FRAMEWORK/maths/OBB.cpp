#include "OBB.h"

namespace sunny
{
	namespace maths
	{
		OBB::OBB(const vec3& center, const vec3& axis, const vec3& len)
		: center(vec3(center)), axis(vec3(axis)), len(vec3(len))
		{

		}

		bool OBB::Intersects(const OBB& other) const
		{

		}
	}
}