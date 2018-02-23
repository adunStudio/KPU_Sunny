#include "Camera.h"

namespace sunny
{
	namespace graphics
	{
		Camera::Camera(const maths::mat4& projectionMatrix) : m_projectionMatrix(projectionMatrix)
		{
			m_viewMatrix = maths::mat4::Identity();
			m_position = maths::vec3();
			m_rotation = maths::vec3();
		}
	}
}