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

		enum PSSunnyForwardUniformIndices : int
		{
			PSSunnyForwardUniformIndex_Lights      = 0,
			PSSunnyForwardUniformIndex_Color       = 1,
			PSSunnyForwardUniformIndex_HasTexture  = 2,
			PSSunnyForwardUniformIndex_Size
		};

		enum PSSunnyDeferredUniformIndices : int
		{
			PSSunnyDeferredUniformIndex_Color	   = 0,
			PSSunnyDeferredUniformIndex_HasTexture = 1,
			PSSunnyDeferredUniformIndex_Size
		};

		enum PSSunnyLightUniformIndices : int
		{
			PSSunnyLightUniformIndex_Lights         = 0,
			PSSunnyLightUniformIndex_CameraPosition = 1,
			PSSunnyLightUniformIndex_Size
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
			m_forwardCommandQueue.reserve(1000);
			m_deferredCommandQueue.reserve(1000);

			m_gBuffer = new  GBuffer();

			m_default_forward_shader  = ShaderFactory::Default3DForwardShader();
			m_default_deferred_shader = ShaderFactory::Default3DDeferredShader();
			m_default_light_shader    = ShaderFactory::Default3DLightShader();

			/* ¹öÅØ½º ¼ÎÀÌ´õ */
			m_VSSunnyUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::vec3);
			m_VSSunnyUniformBuffer = new unsigned char[m_VSSunnyUniformBufferSize];
			memset(m_VSSunnyUniformBuffer, 0, m_VSSunnyUniformBufferSize);

			m_VSSunnyUniformBufferOffsets.resize(VSSunnyUniformIndex_Size);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix] = 0;
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix]       = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix] + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix]      = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix] + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition]   = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix] + sizeof(maths::mat4);

			/* ÇÈ¼¿ ¼ÎÀÌ´õ (Forward) */
			m_PSSunnyForwardUniformBufferSize = sizeof(Light) + sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyForwardUniformBuffer = new unsigned char[m_PSSunnyForwardUniformBufferSize];
			memset(m_PSSunnyForwardUniformBuffer, 0, m_PSSunnyForwardUniformBufferSize);

			m_PSSunnyForwardUniformBufferOffsets.resize(PSSunnyForwardUniformIndex_Size);
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights]     = 0;
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color]      = m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights] + sizeof(Light);
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_HasTexture] = m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color] + sizeof(maths::vec4);
		
			/* ÇÈ¼¿ ¼ÎÀÌ´õ (Deferred) */
			m_PSSunnyDeferredUniformBufferSize = sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyDeferredUniformBuffer = new unsigned char[m_PSSunnyDeferredUniformBufferSize];
			memset(m_PSSunnyDeferredUniformBuffer, 0, m_PSSunnyDeferredUniformBufferSize);

			m_PSSunnyDeferredUniformBufferOffsets.resize(PSSunnyDeferredUniformIndex_Size);
			m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color]      = 0;
			m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_HasTexture] = m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color] + sizeof(maths::vec4);
		
			/* ÇÈ¼¿ ¼ÎÀÌ´õ (Light) */
			m_PSSunnyLightUniformBufferSize = sizeof(Light) + sizeof(maths::vec3);
			m_PSSunnyLightUniformBuffer = new unsigned char[m_PSSunnyLightUniformBufferSize];
			memset(m_PSSunnyLightUniformBuffer, 0, m_PSSunnyLightUniformBufferSize);

			m_PSSunnyLightUniformBufferOffsets.resize(PSSunnyLightUniformIndex_Size);
			m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights] = 0;
			m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_CameraPosition] = m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights] + sizeof(Light);
		}

		void Renderer3D::Begin()
		{
			//directx::Renderer::SetViewport(0, 0, m_screenBufferWidth, m_screenBufferHeight);
			m_deferredCommandQueue.clear();
			m_forwardCommandQueue.clear();
			m_sunnyUniforms.clear();
		}

		void Renderer3D::BeginScene(Camera* camera)
		{
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix],       &camera->GetViewMatrix(),       sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition],   &camera->GetPosition(),         sizeof(maths::vec3));

			memcpy(m_PSSunnyLightUniformBuffer + m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_CameraPosition],   &camera->GetPosition(), sizeof(maths::vec3));
		}

		void Renderer3D::Submit(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);
		}

		void Renderer3D::Submit(const RenderCommand& command)
		{
			if (command.color.w == 1.0f)
				m_deferredCommandQueue.push_back(command);
			else
				m_forwardCommandQueue.push_back(command);
		}

		void Renderer3D::SubmitRenderable3D(Renderable3D* renderable)
		{
			RenderCommand command;

			command.renderable3d = renderable;
			command.color        = renderable->GetColor();
			command.transform    = renderable->GetComponent<component::TransformComponent>()->GetTransform();
			command.hasTexture   = renderable->GetHasTexture();
			command.shader       = renderable->GetShader() ? renderable->GetShader() : m_default_forward_shader;

			Submit(command);
		}

		void Renderer3D::SubmitGroup3D(Group3D* group3d)
		{
			// À§Ä¡¿Í ¼ÎÀÌ´õ´Â ±×·ì¿¡ Á¾¼ÓµÈ´Ù.
			const maths::mat4 groupTransform = group3d->GetComponent<component::TransformComponent>()->GetTransform();
			directx::Shader* groupShader = group3d->GetShader() ? group3d->GetShader() : m_default_forward_shader;

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
			const auto& lights = lightSetup.GetLights();

			if (lights.size() != 1)
			{
				// debug system
				std::cout << "one light is needed" << std::endl;
				exit(1);
			}

			for (unsigned int i = 0; i < lights.size(); ++i)
			{
				memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights], lights[i], sizeof(Light));
				memcpy(m_PSSunnyLightUniformBuffer + m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights], lights[i], sizeof(Light));
			}
		}

		void Renderer3D::EndScene()
		{

		}

		void Renderer3D::End()
		{

		}

		void Renderer3D::Present()
		{
			DeferredPresentInternal();

			ForwardPresentInternal();
		}

		void Renderer3D::ForwardPresentInternal()
		{
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(true);

			for (unsigned int i = 0; i < m_forwardCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_forwardCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
				memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color], &command.color, sizeof(maths::vec4));
				memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

				command.shader->Bind();

				SetSunnyVSUniforms(command.shader);
				SetSunnyForwardUniforms(command.shader);

				command.renderable3d->Render();
			}

		}

		void Renderer3D::DeferredPresentInternal()
		{
			m_gBuffer->Bind();
			directx::Context::GetDeviceContext()->ClearDepthStencilView(directx::DeferredBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			m_gBuffer->UnBind();
			return;

			directx::Renderer::SetDepthTesting(false);
			directx::Renderer::SetBlend(false);

			m_gBuffer->Bind();

			for (unsigned int i = 0; i < m_deferredCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_deferredCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
				memcpy(m_PSSunnyDeferredUniformBuffer + m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color], &command.color, sizeof(maths::vec4));
				memcpy(m_PSSunnyDeferredUniformBuffer + m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

				command.shader = m_default_deferred_shader;
				command.shader->Bind();

				SetSunnyVSUniforms(command.shader);
				SetSunnyDeferredUniforms(command.shader);

				command.renderable3d->Render();
			}
			float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			//directx::Context::GetDeviceContext()->ClearRenderTargetView(directx::DeferredBuffer::GetBuffer(0), color);
			//directx::Context::GetDeviceContext()->ClearRenderTargetView(directx::DeferredBuffer::GetBuffer(1), color);
			//directx::Context::GetDeviceContext()->ClearRenderTargetView(directx::DeferredBuffer::GetBuffer(2), color );
			directx::Context::GetDeviceContext()->ClearRenderTargetView(directx::Context::GetBackBuffer(), color);
			//directx::Context::GetDeviceContext()->ClearDepthStencilView(directx::DeferredBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			m_gBuffer->UnBind();

			directx::Context::GetDeviceContext()->ClearDepthStencilView(directx::DeferredBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			m_default_light_shader->Bind();

			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::POSITION);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::DIFFUSE);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::NORMAL);
			m_gBuffer->SetGBufferSampler();

			SetSunnyLightUniforms(m_default_light_shader);

			directx::Renderer::SetDepthTesting(false);

		//	directx::Renderer::GetRenderer()->Clear(RENDERER_BUFFER_DEPTH | RENDERER_BUFFER_DEFERRED);
			m_gBuffer->Draw();
		}

		void Renderer3D::SetSunnyVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyUniformBuffer, m_VSSunnyUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyForwardUniforms(directx::Shader* shader)
		{
			shader->SetPSSystemUniformBuffer(m_PSSunnyForwardUniformBuffer, m_PSSunnyForwardUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyDeferredUniforms(directx::Shader* shader)
		{
			shader->SetPSSystemUniformBuffer(m_PSSunnyDeferredUniformBuffer, m_PSSunnyDeferredUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyLightUniforms(directx::Shader* shader)
		{
			shader->SetPSSystemUniformBuffer(m_PSSunnyLightUniformBuffer, m_PSSunnyLightUniformBufferSize, 0);
		}
	}
}