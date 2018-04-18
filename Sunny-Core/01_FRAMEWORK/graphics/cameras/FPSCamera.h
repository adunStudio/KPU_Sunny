#pragma once

#include "Camera.h"

namespace sunny
{
	namespace graphics
	{
		class FPSCamera : public Camera
		{
		private:
			float m_mouseSensitivity;
			float m_speed, m_sprintSpeed;
			float m_pitch, m_yaw;
			bool m_mouseWasGrabbed;

		public:
			FPSCamera(const maths::mat4& projectionMatrix);
			~FPSCamera();

			void Focus() override;
			void Update() override;

		private:
			maths::Quaternion GetOrientation() const;
			maths::vec3 GetForwardDirection(const maths::Quaternion& orientation) const;
			maths::vec3 GetUpDirection     (const maths::Quaternion& orientation) const;
			maths::vec3 GetRightDirection  (const maths::Quaternion& orientation) const;
		};
	}
}