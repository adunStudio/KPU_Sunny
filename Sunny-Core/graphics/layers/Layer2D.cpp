#include "Layer2D.h"

#include "../../app/Window.h"
#include "../../app/Application.h"

namespace sunny
{
	namespace graphics
	{
		Layer2D::Layer2D(const maths::mat4& projectionMatrix)
		{
			float width  = Application::GetApplication().GetWindowWidth();
			float height = Application::GetApplication().GetWindowHeight();
			float aspect = width / height;

			//m_camera = new OrthographicCamera(maths::mat4::Orthographic(-10.0f * aspect, 10.0f * aspect, -10.0f, 10.0f, -1.0f, 1.0f));
			m_camera = new OrthographicCamera(maths::mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
			m_renderer = new Renderer2D(width, height);
			
			m_renderer->SetCamera(m_camera);
		}

		Layer2D::~Layer2D()
		{
			delete m_renderer;
		}

		Renderable2D* Layer2D::Add(Renderable2D* renderable)
		{
			m_renderables.push_back(renderable);

			return renderable;
		}

		void Layer2D::Init()
		{
			OnInit(*m_renderer);
		}

		void Layer2D::OnInit(Renderer2D& renderer)
		{

		}

		void Layer2D::OnUpdateInternal(const utils::Timestep& ts)
		{
			OnUpdate(ts);
		}

		void Layer2D::OnRender(Renderer2D& renderer)
		{

		}

		void Layer2D::OnRender()
		{
			m_camera->Update();

			m_renderer->Begin();
			
			for (const Renderable2D* renderable : m_renderables)
			{
				renderable->Submit(m_renderer);
			}

			m_renderer->End();
			m_renderer->Present();

			OnRender(*m_renderer);

			//m_renderables.clear();
		}

		bool Layer2D::OnResize(unsigned int width, unsigned int height)
		{
			m_renderer->SetScreenSize(maths::tvec2<unsigned int>(width, height));
			return false;
		}
	}
}