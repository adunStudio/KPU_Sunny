#include "MayaCamera.h"

namespace sunny
{
	namespace graphics
	{
		MayaCamera::MayaCamera(const maths::mat4& projectionMatrix) : Camera(projectionMatrix)
		{
			/* default */
			m_panSpeed = 0.0015f;
			m_rotationSpeed = 0.002f;
			m_zoomSpeed = 0.2f;

			m_position = maths::vec3( 0.0f, 25.0f, -25.0f);
			m_rotation = maths::vec3(90.0f,  0.0f,   0.0f);

			m_focalPoint = maths::vec3::Zero();

			m_distance = m_position.Distance(m_focalPoint);

			m_yaw = 3.0f * maths::SUNNY_PI / 4.0f;
			m_pitch = maths::SUNNY_PI / 4.0f;
		}

		void MayaCamera::Focus()
		{

		}

		void MayaCamera::Update()
		{
			m_position = CalcuatePosition();

			maths::Quaternion orientation = GetOrientation();
			m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);
		
			m_viewMatrix = maths::mat4::Translate(maths::vec3(0, 0, 1)) * maths::mat4::Rotate(orientation.Conjugate()) * maths::mat4::Translate(-m_position);
		}

		maths::vec3 MayaCamera::GetUpDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::YAxis());
		}

		maths::vec3 MayaCamera::GetRightDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::XAxis());
		}

		maths::vec3 MayaCamera::GetForwardDiretion()
		{
			return maths::Quaternion::Rotate(GetOrientation(), -maths::vec3::ZAxis());
		}

		maths::vec3 MayaCamera::CalcuatePosition()
		{
			return m_focalPoint - GetForwardDiretion() * m_distance;
		}

		maths::Quaternion MayaCamera::GetOrientation()
		{
			return maths::Quaternion::RotationY(-m_yaw) * maths::Quaternion::RotationX(-m_pitch);
		}
	}
}