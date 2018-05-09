#include "Layer3D.h"

namespace sunny
{
	namespace graphics
	{
		Layer3D::Layer3D(Renderer3D * renderer)
		: m_renderer(renderer), m_camera(new MayaCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f)))
		{
			m_camera->Focus();
		}

		Layer3D::~Layer3D()
		{
			for (unsigned int i = 0; i < m_renderables.size(); ++i)
				delete m_renderables[i];

			m_renderables.clear();

			delete m_camera;
		}

		Renderable3D* Layer3D::Add(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);

			return renderable;
		}

		Renderable3D* Layer3D::AddStatic(Renderable3D* renderable)
		{
			m_renderer->SubmitStatic3D(renderable);

			return renderable;
		}

		Group3D* Layer3D::Add(Group3D* group3d)
		{
			m_group3ds.push_back(group3d);

			return group3d;
		}

		void Layer3D::SetCamera(Camera * camera)
		{
			delete m_camera;

			m_camera = camera;

			m_camera->Focus();
		}

		void Layer3D::Init()
		{
			OnInit(*m_renderer);
		}
		
		void Layer3D::OnInit(Renderer3D& renderer)
		{

		}

		void Layer3D::OnUpdateInternal(const utils::Timestep& ts)
		{
			OnUpdate(ts);
		}

		void Layer3D::OnRender(Renderer3D& renderer)
		{
			
		}

		void Layer3D::OnRender()
		{
			m_camera->Update();

			m_renderer->Begin();
			m_renderer->BeginScene(m_camera);

			for (LightSetup* lightSetup : m_lightSetupStack)
			{
				m_renderer->SubmitLight(*lightSetup);
			}

			for (Renderable3D* renderable3d : m_renderables)
			{
				if(renderable3d->GetVisible())
					m_renderer->SubmitRenderable3D(renderable3d);
			}

			for (Group3D* group3d : m_group3ds)
			{
				if (group3d->GetVisible())
					m_renderer->SubmitGroup3D(group3d);
			}

			m_renderer->EndScene(m_camera);
			m_renderer->End();

			m_renderer->Present();

			OnRender(*m_renderer);
		}

		void Layer3D::PushLightSetup(LightSetup* lightSetup)
		{
			m_lightSetupStack.push_back(lightSetup);
		}

		LightSetup* Layer3D::PopLightSetup()
		{
			LightSetup* lightSetup = m_lightSetupStack.back();
			m_lightSetupStack.pop_back();

			return lightSetup;
		}

		void Layer3D::SetSkybox(Renderable3D* renderable)
		{
			m_renderer->SetSkybox(renderable);
		}

		bool Layer3D::OnResize(unsigned int width, unsigned int height)
		{
			m_renderer->SetScreenBufferSize(width, height);
			return false;
		}
	}
}