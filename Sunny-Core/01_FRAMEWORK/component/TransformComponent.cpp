#include "TransformComponent.h"

namespace sunny
{
	namespace component
	{
		TransformComponent::TransformComponent(const maths::mat4& transform)
		: m_transform(transform)
		{
			InitTransform();
		}

		void TransformComponent::InitTransform()
		{
			// https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati

			m_position = vec3(0, 0, 0);
			m_rotation = vec3(0, 0, 0);
			m_scale    = vec3(1, 1, 1);

			/*m_position = vec3(m_transform.rows[0].w, m_transform.rows[1].w, m_transform.rows[2].w);

			float Sx = vec3(m_transform.rows[0].x, m_transform.rows[1].x, m_transform.rows[2].x).Magnitude();
			float Sy = vec3(m_transform.rows[0].y, m_transform.rows[1].y, m_transform.rows[2].y).Magnitude();
			float Sz = vec3(m_transform.rows[0].z, m_transform.rows[1].z, m_transform.rows[2].z).Magnitude();

			vec4 row0 = vec4(m_transform.rows[0].x / Sx, m_transform.rows[1].x / Sx, m_transform.rows[2].x / Sx, 0);
			vec4 row1 = vec4(m_transform.rows[0].y / Sy, m_transform.rows[1].y / Sy, m_transform.rows[2].y / Sx, 0);
			vec4 row2 = vec4(m_transform.rows[0].z / Sz, m_transform.rows[1].z / Sz, m_transform.rows[2].z / Sz, 0);
			vec4 row3 = vec4(0, 0, 0, 1);

			m_rotation = mat4(row0, row1, row2, row3);
			m_scale = vec3(Sx, Sy, Sz);*/
		}

		void TransformComponent::CalTransform()
		{
			m_transform = mat4::Identity() * mat4::Translate(m_position) * mat4::Rotate(m_rotation.x, vec3(1, 0, 0)) * mat4::Rotate(m_rotation.y, vec3(0, 1, 0)) * mat4::Rotate(m_rotation.z, vec3(0, 0, 1)) * mat4::Scale(m_scale);
		}

		void TransformComponent::Translate(const maths::vec3& translation)
		{
			m_position += translation;

			CalTransform();
		}

		void TransformComponent::Rotate(const maths::vec3& rotation)
		{
			m_rotation += rotation;

			CalTransform();
		}

		void TransformComponent::SetPosition(const maths::vec3& position)
		{
			m_position = position;

			CalTransform();
		}

		void TransformComponent::SetHeight(float y)
		{
			m_position.y = y;

			CalTransform();
		}

		void TransformComponent::SetRotation(const maths::vec3& rotation)
		{
			m_rotation = rotation;

			CalTransform();
		}

		void TransformComponent::SetScale(const maths::vec3& scale)
		{
			m_scale = scale;

			CalTransform();
		}
	}
}