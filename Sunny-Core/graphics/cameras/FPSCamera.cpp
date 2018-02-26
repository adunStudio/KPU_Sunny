#include "FPSCamera.h"

#include "../../app/Input.h"
#include "../../app/Application.h"

namespace sunny
{
	namespace graphics
	{
		FPSCamera::FPSCamera(const maths::mat4& projectionMatrix)
		: Camera(projectionMatrix), m_mouseSensitivity(0.002f), m_speed(0.4f), m_sprintSpeed(m_speed * 4.0f), m_mouseWasGrabbed(false)
		{
			m_position = maths::vec3(0.0f, 25.0f, -25.0f);
			m_rotation = maths::vec3(90.0f, 0.0f, 0.0f);
			m_yaw = 2.4f;
			m_pitch = 0.7f;
		}

		FPSCamera::~FPSCamera()
		{

		}

		void FPSCamera::Focus()
		{
			Input::GetInputManager()->SetMouseCursor(SUNNY_NO_CURSOR);
		}

		void FPSCamera::Update()
		{
			maths::vec2 windowSize = Application::GetApplication().GetWindowSize();
			maths::vec2 windowCenter = maths::vec2((float)(windowSize.x / 2.0f), (float)(windowSize.y / 2.0f));
		
			if (Input::IsMouseButtonPressed(SUNNY_MOUSE_RIGHT))
			{
				if (!Input::GetInputManager()->IsMouseGrabbed())
				{
					Input::GetInputManager()->SetMouseGrabbed(true);
					Input::GetInputManager()->SetMouseCursor(SUNNY_NO_CURSOR);
				}
			}

			if (Input::GetInputManager()->IsMouseGrabbed())
			{
				maths::vec2 mouse = Input::GetInputManager()->GetMousePosition();
				mouse.x -= windowCenter.x;
				mouse.y -= windowCenter.y;

				if (m_mouseWasGrabbed)
				{
					m_yaw   += mouse.x * m_mouseSensitivity;
					m_pitch += mouse.y * m_mouseSensitivity;
				}

				m_mouseWasGrabbed = true;

				Input::GetInputManager()->SetMousePosition(windowCenter);

				maths::Quaternion orientation = GetOrientation();
				m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);
			
				maths::vec3 forward = GetForwardDirection(orientation);
				maths::vec3 right = GetRightDirection(orientation);
				maths::vec3 up = maths::vec3::YAxis();

				float speed = Input::IsKeyPressed(SUNNY_KEY_SHIFT) ? m_sprintSpeed : m_speed;

				if (Input::IsKeyPressed(SUNNY_KEY_W))
					m_position += forward * speed;
				else if (Input::IsKeyPressed(SUNNY_KEY_S))
					m_position -= forward * speed;

				if (Input::IsKeyPressed(SUNNY_KEY_A))
					m_position -= right * speed;
				else if (Input::IsKeyPressed(SUNNY_KEY_D))
					m_position += right * speed;

				if (Input::IsKeyPressed(SUNNY_KEY_SPACE))
					m_position += up * speed;
				else if (Input::IsKeyPressed(SUNNY_KEY_CONTROL))
					m_position -= up * speed;

				maths::mat4 rotation = maths::mat4::Rotate(orientation.Conjugate());
				maths::mat4 translation = maths::mat4::Translate(-m_position);
				m_viewMatrix = rotation * translation;
			}

			if (Input::IsKeyPressed(SUNNY_KEY_ESCAPE))
			{
				Input::GetInputManager()->SetMouseGrabbed(false);
				Input::GetInputManager()->SetMouseCursor(1);
				m_mouseWasGrabbed = false;
			}
		}

		maths::Quaternion FPSCamera::GetOrientation() const
		{
			return maths::Quaternion::RotationY(-m_yaw) * maths::Quaternion::RotationX(-m_pitch);
		}

		maths::vec3 FPSCamera::GetForwardDirection(const maths::Quaternion& orientation) const
		{
			return maths::Quaternion::Rotate(orientation, -maths::vec3::ZAxis());
		}

		maths::vec3 FPSCamera::GetUpDirection(const maths::Quaternion& orientation) const
		{
			return maths::Quaternion::Rotate(orientation, maths::vec3::YAxis());
		}

		maths::vec3 FPSCamera::GetRightDirection(const maths::Quaternion& orientation) const
		{
			return maths::Quaternion::Rotate(orientation, maths::vec3::XAxis());
		}
	}
}