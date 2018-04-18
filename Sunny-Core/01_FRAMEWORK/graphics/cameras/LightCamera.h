#pragma once

#include "../../include.h"
#include "Camera.h"

namespace sunny
{
	namespace graphics
	{
		class LightCamera : public Camera
		{
		private:
			bool m_panning;
			bool m_rotating;

			float m_distance;

			float m_pitch, m_yaw;

		public:
			LightCamera(const maths::mat4& projectionMatrix);

			void Focus() override;
			void Update() override;

			inline float GetDistance() const { return m_distance; }
			inline void SetDistance(float distance) { m_distance; }

		private:
			
			maths::vec3 GetUpDirection();
			maths::vec3 GetRightDirection();
			maths::vec3 GetForwardDirection();

			maths::vec3 CalcuatePosition();
			maths::Quaternion GetOrientation();
		};
	}
}