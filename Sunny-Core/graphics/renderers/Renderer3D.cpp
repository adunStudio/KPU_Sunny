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
			PSSunnyUniformIndex_Color       = 0,
			PSSunnyUniformIndex_HasTexture  = 1,
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
			m_commandQueue.reserve(1000);

			m_gBuffer = new  GBuffer();

			m_default_forward_shader  = ShaderFactory::Default3DForwardShader();
			m_default_deferred_shader = ShaderFactory::Default3DDeferredShader();
			m_default_light_shader    = ShaderFactory::Default3DLightShader();

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
			//m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights] = 0;
			//m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color] = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights] + sizeof(Light);
			//m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture] = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color] + sizeof(maths::vec4);
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color]      = 0;
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture] = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color] + sizeof(maths::vec4);
		}

		void Renderer3D::Begin()
		{
			//directx::Renderer::SetViewport(0, 0, m_screenBufferWidth, m_screenBufferHeight);
			m_commandQueue.clear();
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

		void Renderer3D::Submit(const RenderCommand& command)
		{
			m_commandQueue.push_back(command);
		}

		void Renderer3D::SubmitRenderable3D(Renderable3D* renderable)
		{
			RenderCommand command;

			command.renderable3d = renderable;
			command.color        = renderable->GetColor();
			command.transform    = renderable->GetComponent<component::TransformComponent>()->GetTransform();
			command.hasTexture   = renderable->GetHasTexture();
			command.shader       = renderable->GetShader() ? renderable->GetShader() : m_default_deferred_shader;

			Submit(command);
		}

		void Renderer3D::SubmitGroup3D(Group3D* group3d)
		{
			// 위치와 셰이더는 그룹에 종속된다.
			const maths::mat4 groupTransform = group3d->GetComponent<component::TransformComponent>()->GetTransform();
			directx::Shader* groupShader = group3d->GetShader() ? group3d->GetShader() : m_default_deferred_shader;

			RenderCommand command;
			command.transform = groupTransform;
			command.shader = groupShader;;

			for (Renderable3D* renderable : group3d->GetRenderableList())
			{
				command.renderable3d = renderable;
				command.color = renderable->GetColor();
				command.hasTexture = renderable->GetHasTexture();
			
				Submit(command);
			}
		}

		void Renderer3D::SubmitLight(const LightSetup& lightSetup)
		{
			/*const auto& lights = lightSetup.GetLights();

			if (lights.size() != 1)
			{
				// debug system
				std::cout << "one light is needed" << std::endl;
				exit(1);
			}

			for (unsigned int i = 0; i < lights.size(); ++i)
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights], lights[i], sizeof(Light));
				*/
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
			directx::Renderer::SetBlend(true);

			m_gBuffer->Bind();

			for (unsigned int i = 0; i < m_commandQueue.size(); ++i)
			{
				RenderCommand& command = m_commandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color], &command.color, sizeof(maths::vec4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture], &command.hasTexture, sizeof(float));
			
				command.shader = m_default_deferred_shader;
				command.shader->Bind();

				SetSunnyUniforms(command.shader);

				command.renderable3d->Render();
			}

			m_gBuffer->UnBind();

			directx::Renderer::SetBlend(false);

			m_default_light_shader->Bind();

			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::POSITION);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::DIFFUSE );
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::NORMAL);
			m_gBuffer->SetGBufferSampler();
			
			m_gBuffer->Draw();
		}

		void Renderer3D::SetSunnyUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyUniformBuffer, m_VSSunnyUniformBufferSize, 0);
			shader->SetPSSystemUniformBuffer(m_PSSunnyUniformBuffer, m_PSSunnyUniformBufferSize, 0);
		}
	}
}