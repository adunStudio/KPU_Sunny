#include "Ray.h"

namespace sunny
{
	namespace maths
	{
		Ray::Ray() 
		: pos(vec3()), dir(vec3())
		{
		};

		Ray::Ray(const vec3& pos, const vec3& dir)
		: pos(vec3(pos)), dir(vec3(dir))
		{

		};

		bool Ray::Intersect(const Ray& other)
		{
			return false;
		};

		bool Ray::Intersect(const AABB& other)
		{
			return false;
		};

		bool Ray::Intersect(const OBB& other)
		{
			return false;
		};

		std::ostream& operator<<(std::ostream& stream, const Ray& ray)
		{
			stream << "Ray: (" << ray.pos.x << ", " << ray.pos.y << ", " << ray.pos.z << ") -> (" << ray.dir.x << ", " << ray.dir.y << ", " << ray.dir.z << ")";
			return stream;
		}
	}
}