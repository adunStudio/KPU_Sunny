#pragma once

#include "../../sunny.h"
#include "Camera.h"

namespace sunny
{
	namespace graphics
	{
		class MayaCamera : public Camera
		{
		private:
			bool m_panning;
			bool m_rotating;

			maths::vec2 m_initialMousePosition;
			maths::vec3 m_initialFocalPoint;
			maths::vec3 m_initialRotation;

			float m_distance;

			float m_panSpeed;
			float m_rotationSpeed;
			float m_zoomSpeed;

			float m_pitch, m_yaw;

		public:
			MayaCamera(const maths::mat4& projectionMatrix);

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