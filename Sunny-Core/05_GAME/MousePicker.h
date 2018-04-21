#pragma once

#include <SUNNY.h>


namespace game
{
	class MousePicker
	{
	private:
		vec3 m_ray;

		mat4 m_projection;
		mat4 m_view;

		vec2 m_mouse;

		Camera* m_camera;
	private:
		void CalcuateMouseRay()
		{
			float mouseX = m_mouse.x;
			float mouseY = m_mouse.y;

			vec2 normalizeCoords = GetNormalizedDeviceCoords(mouseX, mouseY);
			vec3 clipCoords = vec3(normalizeCoords.x, normalizeCoords.y, -1.f);
			vec3 eyeCoords = ToEyeCoords(clipCoords);
			vec3 worldRay = ToWorldCoords(eyeCoords);


			m_ray.x = worldRay.x;
			m_ray.y = worldRay.y;
				m_ray.z = worldRay.z;
//			std::cout << worldRay << std::endl;

	//		return worldRay;
		}

		const vec2& GetNormalizedDeviceCoords(float mouseX, float mouseY)
		{
			float x = (2.f * mouseX) / Window::GetWindowClass()->GetWidth()  - 1.f;
			float y = 1.f - (2.f * mouseY) / Window::GetWindowClass()->GetHeight();

			return vec2(x, y);
		}

		const vec3& ToEyeCoords(const vec3& clipCoords)
		{
			mat4 invertedProjection = mat4::Invert(m_projection);
			mat4 eyeProjection = invertedProjection.Translate(clipCoords);
			vec3 eyePosition = eyeProjection.GetPosition();
			eyePosition.z = -1.f;

			return eyePosition;
		}

		const vec3& ToWorldCoords(const vec3& eyeCoords)
		{
			mat4 invertedView = mat4::Invert(m_view);
			mat4 rayProjection = invertedView.Translate(eyeCoords);
			vec3 mouseRay = rayProjection.GetPosition();
			mouseRay = mouseRay.Normalize();
		
			return mouseRay;
		}

	public:
		MousePicker(Camera* camera)
		: m_camera(camera)
		{
			m_projection = m_camera->GetProjectionMatrix();
			m_view = m_camera->GetViewMatrix();
		}

		const vec3& GetRay()
		{
			return m_ray;
		}

		void Update(const vec2& mouse)
		{
			m_view = m_camera->GetViewMatrix();

			m_mouse.x = mouse.x;
			m_mouse.y = mouse.y;
			CalcuateMouseRay();
			//m_ray = 
		}
	};
}