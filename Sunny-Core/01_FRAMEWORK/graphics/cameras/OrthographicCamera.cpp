#include "OrthographicCamera.h"

namespace sunny
{
	namespace graphics
	{
		OrthographicCamera::OrthographicCamera(const maths::mat4& projectionMatrix)
		: Camera(projectionMatrix)
		{

		}
	}
}