#include "TransformComponent.h"

namespace sunny
{
	namespace component
	{
		TransformComponent::TransformComponent(const maths::mat4& transform)
		: m_transform(transform)
		{
			// https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati

			m_position = vec3(transform.rows[0].w, transform.rows[1].w, transform.rows[2].w);
			
			float Sx = vec3(transform.rows[0].x, transform.rows[1].x, transform.rows[2].x).Magnitude();
			float Sy = vec3(transform.rows[0].y, transform.rows[1].y, transform.rows[2].y).Magnitude();
			float Sz = vec3(transform.rows[0].z, transform.rows[1].z, transform.rows[2].z).Magnitude();
			
			vec4 row0 = vec4(transform.rows[0].x / Sx, transform.rows[1].x / Sx, transform.rows[2].x / Sx, 0);
			vec4 row1 = vec4(transform.rows[0].y / Sy, transform.rows[1].y / Sy, transform.rows[2].y / Sx, 0);
			vec4 row2 = vec4(transform.rows[0].z / Sz, transform.rows[1].z / Sz, transform.rows[2].z / Sz, 0);
			vec4 row3 = vec4(0, 0, 0, 1);

			m_rotation = mat4(row0, row1, row2, row3);
			m_scale    = vec3(Sx, Sy, Sz);
		}

		void TransformComponent::SetTransform()
		{
			m_transform = mat4::Identity() * mat4::Translate(m_position) * m_rotation * mat4::Scale(m_scale);
		}

		void TransformComponent::Translate(const maths::vec3& translation)
		{
			m_position += translation;

			SetTransform();
		}

		void TransformComponent::Rotate(float angle, const maths::vec3& axis)
		{
			m_rotation *= mat4::Rotate(angle, axis);

			SetTransform();
		}

		void TransformComponent::SetPosition(const maths::vec3& position)
		{
			m_position = position;

			SetTransform();
		}

		void TransformComponent::SetRotation(const maths::mat4& rotation)
		{
			m_rotation = rotation;

			SetTransform();
		}

		void TransformComponent::SetScale(const maths::vec3& scale)
		{
			m_scale = scale;

			SetTransform();
		}
	}
}