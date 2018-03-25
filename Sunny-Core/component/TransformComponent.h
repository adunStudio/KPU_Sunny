#pragma once

#include "Component.h"
#include "../maths/maths.h"

namespace sunny
{
	namespace component
	{
		using namespace maths;

		class TransformComponent : public Component
		{
		private:
			mat4 m_transform;

		private:
			vec3 m_position;
			mat4 m_rotation;
			vec3 m_scale;

		private:
			void InitTransform();

			void CalTransform();

		private:
			inline void SetTransform(const mat4& transform) { m_transform = transform; }

		public:
			TransformComponent(const mat4& transform);

			inline const mat4& GetTransform() const { return m_transform; }

			void Translate(const vec3& translation);
			void Rotate(float angle, const vec3& axis);

			void SetPosition(const vec3& position);
			void SetRotation(const mat4& rotation);
			void SetScale   (const vec3& scale);

			void SetHeight(float y);

			inline vec3& GetPosition() { return m_position; }
			inline mat4& GetRotation() { return m_rotation; }
			inline vec3& GetScale()    { return m_scale;    }

			static ComponentType* GetStaticType()
			{
				static ComponentType type({ "Transform" });

				return &type;
			}

			inline virtual ComponentType* GetType() const override
			{
				return GetStaticType(); 
			}
		};
	}
}