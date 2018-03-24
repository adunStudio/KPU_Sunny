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
		public:
			mat4 transform;

		private:
			vec3 m_position;
			mat4 m_rotation;
			vec3 m_scale;

		private:
			void SetTransform();

		public:
			TransformComponent(const mat4& transform);

			void Translate(const vec3& translation);
			void Rotate(float angle, const vec3& axis);

			void SetPosition(const vec3& position);
			void SetRotation(const mat4& rotation);
			void SetScale   (const vec3& scale);

			inline const vec3& GetPosition() const { return m_position; }
			inline const mat4& GetRotation() const { return m_rotation; }
			inline const vec3& GetScale()    const { return m_scale;    }

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