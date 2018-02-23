#include "Renderer3D.h"

#include "../../app/Application.h"
#include "../../directx/Renderer.h"

namespace sunny
{
	namespace graphics
	{
		enum VSSunnyUniformIndices : int
		{
			VSSunnyUniformIndex_ProjectionMatrix = 0,
			VSSunnyUniformIndex_ViewMatrix       = 1,
			VSSunnyUniformIndex_ModelMatrix      = 2,
			VSSunnyUniformIndex_CameraPosition   = 3,
			VSSunnyUniformIndex_Size
		};

		enum PSSunnyUniformIndices : int
		{
			PSSunnyUniformIndex_Lights      = 0,
			PSSunnyUniformIndex_Color       = 1,
			PSSunnyUniformIndex_HasTexture  = 2,
			PSSunnyUniformIndex_Size
		};

		Renderer3D::Renderer3D()
		{
			SetScreenBufferSize(Application::GetApplication().GetWindowWidth(), Application::GetApplication().GetWindowHeight());
			Init();
		}

		Renderer3D::Renderer3D(unsigned width, unsigned height)
		{
			SetScreenBufferSize(width, height);
			Init();
		}

		void Renderer3D::Init()
		{
			m_shader = ShaderFactory::Default3DShader();

			m_VSSunnyUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::vec3);
			m_VSSunnyUniformBuffer = new unsigned char[m_VSSunnyUniformBufferSize];
			memset(m_VSSunnyUniformBuffer, 0, m_VSSunnyUniformBufferSize);

			m_VSSunnyUniformBufferOffsets.resize(VSSunnyUniformIndex_Size);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix] = 0;
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix] = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix] + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix] = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix] + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition] = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix] + sizeof(maths::mat4);

			m_PSSunnyUniformBufferSize = sizeof(Light) + sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyUniformBuffer = new unsigned char[m_PSSunnyUniformBufferSize];
			memset(m_PSSunnyUniformBuffer, 0, m_PSSunnyUniformBufferSize);

			m_PSSunnyUniformBufferOffsets.resize(PSSunnyUniformIndex_Size);
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights] = 0;
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color] = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights] + sizeof(Light);
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture] = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color] + sizeof(maths::vec4);
		}

		void Renderer3D::Begin()
		{
			//directx::Renderer::SetViewport(0, 0, m_screenBufferWidth, m_screenBufferHeight);
			m_sunnyUniforms.clear();
		}

		void Renderer3D::BeginScene(Camera* camera)
		{
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix],       &camera->GetViewMatrix(),       sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition],   &camera->GetPosition(),         sizeof(maths::vec3));
		}

		void Renderer3D::Submit(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);
		}

		void Renderer3D::EndScene()
		{

		}

		void Renderer3D::End()
		{

		}

		void Renderer3D::Present()
		{
			directx::Renderer::SetDepthTesting(true);
			
			m_shader->Bind();

			for (unsigned int i = 0; i < m_renderables.size(); ++i)
			{
				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &m_renderables[i]->GetTransform() , sizeof(maths::mat4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color]      , &m_renderables[i]->GetColor()     , sizeof(maths::vec4));
				float hasTexture = m_renderables[i]->GetHasTexture();
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture], &hasTexture, sizeof(float));

				SetSunnyUniforms();

				m_renderables[i]->Render();
			}

			m_renderables.clear();
		}

		void Renderer3D::SetSunnyUniforms()
		{
			m_shader->SetVSSystemUniformBuffer(m_VSSunnyUniformBuffer, m_VSSunnyUniformBufferSize, 0);
			m_shader->SetPSSystemUniformBuffer(m_PSSunnyUniformBuffer, m_PSSunnyUniformBufferSize, 0);
		}
	}
}