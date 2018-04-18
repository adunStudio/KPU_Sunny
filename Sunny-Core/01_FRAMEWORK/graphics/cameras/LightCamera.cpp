#include "LightCamera.h"

#include "../../app/Window.h"
#include "../../app/Input.h"

namespace sunny
{
	namespace graphics
	{
		LightCamera::LightCamera(const maths::mat4& projectionMatrix) : Camera(projectionMatrix)
		{
			/*
			m_position = maths::vec3(0.0f, 0.0f, 65.0f);
			m_rotation = maths::vec3(90.0f, 0.0f, 0.0f);

			m_focalPoint = maths::vec3::Zero();

			m_distance = m_position.Distance(m_focalPoint);

			m_yaw = 0.0f; //*maths::SUNNY_PI / 4.0f;
			m_pitch = 0.3f;// maths::SUNNY_PI / 4.0f;
			*/
			
			m_position = maths::vec3(0.0f, 600.0f, 65.0f);
			m_rotation = maths::vec3(90.0f, 0.0f, 0.0f);

			m_focalPoint = maths::vec3::Zero();

			m_distance = m_position.Distance(m_focalPoint);

			m_yaw = 0.0f; //*maths::SUNNY_PI / 4.0f;
			m_pitch = 0.6f;// maths::SUNNY_PI / 4.0f;
			

			m_position = CalcuatePosition();

			maths::Quaternion orientation = GetOrientation();
			m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);

			m_viewMatrix = maths::mat4::Translate(maths::vec3(0, 0, 1)) * maths::mat4::Rotate(orientation.Conjugate()) * maths::mat4::Translate(-m_position);
		}

		void LightCamera::Focus()
		{
			Input::GetInputManager()->SetMouseCursor(1);
		}

		void LightCamera::Update()
		{
			m_position = CalcuatePosition();

			maths::Quaternion orientation = GetOrientation();
			m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);

			m_viewMatrix = maths::mat4::Translate(maths::vec3(0, 0, 1)) * maths::mat4::Rotate(orientation.Conjugate()) * maths::mat4::Translate(-m_position);
		}

		maths::vec3 LightCamera::GetUpDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::YAxis());
		}

		maths::vec3 LightCamera::GetRightDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::XAxis());
		}

		maths::vec3 LightCamera::GetForwardDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), -maths::vec3::ZAxis());
		}

		maths::vec3 LightCamera::CalcuatePosition()
		{
			return m_focalPoint - GetForwardDirection() * m_distance;
		}

		maths::Quaternion LightCamera::GetOrientation()
		{
			return maths::Quaternion::RotationY(-m_yaw) * maths::Quaternion::RotationX(-m_pitch);
		}
	}
}