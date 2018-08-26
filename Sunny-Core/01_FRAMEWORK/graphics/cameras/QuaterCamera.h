#pragma once

#include "../../include.h"
#include "Camera.h"
#include "../renderables/Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		using namespace component;

		class QuaterCamera : public Camera
		{
		private:
			const float m_speed = 3.0f;

			bool m_panning;
			bool m_rotating;
			
			Renderable3D* m_renderable;
			TransformComponent* m_transform;

			maths::vec2 m_initialMousePosition;
			maths::vec3 m_initialFocalPoint;
			maths::vec3 m_initialRotation;

			float m_distance;

			float m_panSpeed;
			float m_rotationSpeed;
			float m_zoomSpeed;

			float m_pitch, m_yaw;

		public:
			QuaterCamera(const maths::mat4& projectionMatrix, Renderable3D* renderable);

			void Focus() override;
			void Update() override;

			inline float GetDistance() const { return m_distance; }
			inline void SetDistance(float distance) { m_distance; }

		private:
			void MousePan(const maths::vec2& delta);
			void MouseRotate(const maths::vec2& delta);
			void MouseZoom(float delta);

			maths::vec3 GetUpDirection();
			maths::vec3 GetRightDirection();
			maths::vec3 GetForwardDirection();

			maths::vec3 CalcuatePosition();
			maths::Quaternion GetOrientation();
		};
	}
}