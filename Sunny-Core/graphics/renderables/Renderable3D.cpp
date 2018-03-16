#include "Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

		Renderable3D::Renderable3D()
		: m_transform(maths::mat4::Identity()), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
			m_position = vec3(0, 0, 0);
			m_rotation = mat4::Identity();
			m_scale = vec3(1, 1, 1);
		}

		Renderable3D::Renderable3D(const maths::mat4& transform)
		: m_transform(transform), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
			m_position = vec3(0, 0, 0);
			m_rotation = mat4::Identity();
			m_scale = vec3(1, 1, 1);
		}

		void Renderable3D::Translate(const maths::vec3& translation)
		{
			m_position += translation;
			
			SetTransform();
		}

		void Renderable3D::Rotate(float angle, const maths::vec3& axis)
		{
			m_rotation *= mat4::Rotate(angle, axis);

			SetTransform();
		}

		void Renderable3D::SetPosition(const maths::vec3& position)
		{
			m_position = position;

			SetTransform();
		}

		void Renderable3D::SetRotation(const maths::mat4& rotation)
		{
			m_rotation = rotation;

			SetTransform();
		}

		void Renderable3D::SetScale(const maths::vec3& scale)
		{
			m_scale = scale;

			SetTransform();
		}

		void Renderable3D::SetTransform()
		{
			m_transform = mat4::Identity() * mat4::Translate(m_position) * m_rotation * mat4::Scale(m_scale);
		}
	}
}
