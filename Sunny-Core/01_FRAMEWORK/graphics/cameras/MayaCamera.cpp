#include "MayaCamera.h"

#include "../../app/Window.h"
#include "../../app/Input.h"

namespace sunny
{
	namespace graphics
	{
		MayaCamera::MayaCamera(const maths::mat4& projectionMatrix) : Camera(projectionMatrix)
		{
		

			m_panSpeed = 0.0015f;
			m_rotationSpeed = 0.002f;
			m_zoomSpeed = 1.0f;

			m_position = maths::vec3(0.0f, 1000.0f, -1165.0f);
			m_rotation = maths::vec3(90.0f, 0.0f, 0.0f);

			m_focalPoint = maths::vec3::Zero();

			m_distance = m_position.Distance(m_focalPoint);

			m_yaw   = 0;
			m_pitch = -1;
		}

		void MayaCamera::Focus()
		{
			//Input::GetInputManager()->SetMouseCursor(1);
		}

		void MayaCamera::Update()
		{
			if (Input::IsKeyPressed(SUNNY_KEY_ALT))
			{
				const maths::vec2& mouse = Input::GetMousePosition();
				maths::vec2 delta = mouse - m_initialMousePosition;
				m_initialMousePosition = mouse;

				if (Input::IsMouseButtonPressed(SUNNY_MOUSE_MIDDLE))
					MousePan(delta);
				else if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT))
					MouseRotate(delta);
				else if (Input::IsMouseButtonPressed(SUNNY_MOUSE_RIGHT))
					MouseZoom(delta.y);
			}

			m_position = CalcuatePosition();

			maths::Quaternion orientation = GetOrientation();
			m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);
		
			m_viewMatrix = maths::mat4::Translate(maths::vec3(0, 0, 1)) * maths::mat4::Rotate(orientation.Conjugate()) * maths::mat4::Translate(-m_position);
		}

		void MayaCamera::MousePan(const maths::vec2& delta)
		{
			m_focalPoint += -GetRightDirection() * delta.x * m_panSpeed * m_distance;
			m_focalPoint += GetUpDirection() * delta.y * m_panSpeed * m_distance;
		}

		void MayaCamera::MouseRotate(const maths::vec2& delta)
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			
			m_yaw += yawSign * delta.x * m_rotationSpeed;
			m_pitch += delta.y * m_rotationSpeed;
		}

		void MayaCamera::MouseZoom(float delta)
		{
			m_distance -= delta * m_zoomSpeed;
			
			if (m_distance < 1.0f)
			{
				m_focalPoint += GetForwardDirection();
				m_distance = 1.0f;
			}
		}

		maths::vec3 MayaCamera::GetUpDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::YAxis());
		}

		maths::vec3 MayaCamera::GetRightDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::XAxis());
		}

		maths::vec3 MayaCamera::GetForwardDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::ZAxis());
		}

		maths::vec3 MayaCamera::CalcuatePosition()
		{
			return m_focalPoint - GetForwardDirection() * m_distance;
		}

		maths::Quaternion MayaCamera::GetOrientation()
		{
			return maths::Quaternion::RotationY(-m_yaw) * maths::Quaternion::RotationX(-m_pitch);
		}
	}
}