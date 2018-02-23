#pragma once

#include "../../sunny.h"
#include "../../maths/maths.h"

namespace sunny
{
	namespace graphics
	{
		class Camera
		{
		protected:
			maths::mat4 m_projectionMatrix;
			maths::mat4 m_viewMatrix;

			maths::vec3 m_position;
			maths::vec3 m_rotation;
			maths::vec3 m_focalPoint;

		public:
			Camera(const maths::mat4& projectionMatirx);

			virtual void Focus()  {}
			virtual void Update() {}

			inline const maths::vec3& GetPosition() const { return m_position; }
			inline void SetPosition(const maths::vec3& position) { m_position = position; }

			inline const maths::vec3& GetRotation() const { return m_rotation; }
			inline void SetRotation(const maths::vec3& rotation) { m_rotation = rotation; }

			inline const maths::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
			inline void SetProjectionMatrix(const maths::mat4& projectionMatrix) { m_projectionMatrix = projectionMatrix; }

			inline void Translate(const maths::vec3& translation) { m_position += translation; }
			inline void Rotate(const maths::vec3& rotation) { m_rotation += rotation; }

			inline void Translate(float x, float y, float z) { m_position += maths::vec3(x, y, z); }
			inline void Rotate(float x, float y, float z) { m_rotation += maths::vec3(x, y, z); }
		
			inline const maths::vec3& GetFocalPoint() const { return m_focalPoint; }

			inline const maths::mat4& GetViewMatrix() { return m_viewMatrix; }
		};
	}
}