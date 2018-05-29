#include "QuaterCamera.h"

#include "../../app/Window.h"
#include "../../app/Input.h"

namespace sunny
{
	namespace graphics
	{
		QuaterCamera::QuaterCamera(const maths::mat4& projectionMatrix, Renderable3D* renderable)
		: Camera(projectionMatrix), m_renderable(renderable)
		{
			m_panSpeed = 0.0015f;
			m_rotationSpeed = 0.002f;
			m_zoomSpeed = 0.2f;

			m_position = maths::vec3(0.0f, 1000.0f, -1165.0f);
			m_rotation = maths::vec3(90.0f, 0.0f, 0.0f);

			m_focalPoint = maths::vec3::Zero();

			m_distance = m_position.Distance(m_focalPoint);

			m_yaw = 0; //maths::SUNNY_PI / 4.0f;
			m_pitch = -1;// maths::SUNNY_PI / 4.0f;
		}

		void QuaterCamera::Focus()
		{
			//Input::GetInputManager()->SetMouseCursor(1);
		}

		void QuaterCamera::Update()
		{
			m_position = CalcuatePosition();
			
			m_position.x = m_renderable->GetTransformComponent()->GetPosition().x;
			m_position.y = m_renderable->GetTransformComponent()->GetPosition().y + 1300;
			m_position.z = m_renderable->GetTransformComponent()->GetPosition().z - 800;
			

			maths::Quaternion orientation = GetOrientation();
			m_rotation = orientation.ToEulerAngles() * (180.0f / maths::SUNNY_PI);

			m_viewMatrix = maths::mat4::Translate(maths::vec3(0, 0, 0)) * maths::mat4::Rotate(orientation.Conjugate()) * maths::mat4::Translate(-m_position);
		}

		void QuaterCamera::MousePan(const maths::vec2& delta)
		{
			m_focalPoint += -GetRightDirection() * delta.x * m_panSpeed * m_distance;
			m_focalPoint += GetUpDirection() * delta.y * m_panSpeed * m_distance;
		}

		void QuaterCamera::MouseRotate(const maths::vec2& delta)
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

			m_yaw += yawSign * delta.x * m_rotationSpeed;
			m_pitch += delta.y * m_rotationSpeed;
		}

		void QuaterCamera::MouseZoom(float delta)
		{
			m_distance -= delta * m_zoomSpeed;

			if (m_distance < 1.0f)
			{
				m_focalPoint += GetForwardDirection();
				m_distance = 1.0f;
			}
		}

		maths::vec3 QuaterCamera::GetUpDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::YAxis());
		}

		maths::vec3 QuaterCamera::GetRightDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), maths::vec3::XAxis());
		}

		maths::vec3 QuaterCamera::GetForwardDirection()
		{
			return maths::Quaternion::Rotate(GetOrientation(), -maths::vec3::ZAxis());
		}

		maths::vec3 QuaterCamera::CalcuatePosition()
		{
			return m_focalPoint - GetForwardDirection() * m_distance;
		}

		maths::Quaternion QuaterCamera::GetOrientation()
		{
			return maths::Quaternion::RotationY(-m_yaw) * maths::Quaternion::RotationX(-m_pitch);
		}
	}
}