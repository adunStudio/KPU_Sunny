#include "Renderer3D.h"

#include "../../app/Application.h"
#include "../../directx/Renderer.h"

namespace sunny
{
	namespace graphics
	{
		enum VSSunnyUniformIndices : int
		{
			VSSunnyUniformIndex_ProjectionMatrix      = 0,
			VSSunnyUniformIndex_ViewMatrix            = 1,
			VSSunnyUniformIndex_ModelMatrix           = 2,
			VSSunnyUniformIndex_CameraPosition        = 3,
			VSSunnyUniformIndex_LightProjectionMatrix = 4,
			VSSunnyUniformIndex_LightViewMatrix       = 5,
			VSSunnyUniformIndex_Size
		};
		
		enum VSSunnyShadowUniformIndices : int
		{
			VSSunnyShadowUniformIndex_ProjectionMatrix      = 0,
			VSSunnyShadowUniformIndex_ViewMatrix            = 1,
			VSSunnyShadowUniformIndex_ModelMatrix           = 2,
			VSSunnyShadowUniformIndex_LightProjectionMatrix = 3,
			VSSunnyShadowUniformIndex_LightViewMatrix       = 4,
			VSSunnyShadowUniformIndex_Size
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

		bool Renderer3D::DEFERRED_MODE = false;

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

			m_gBuffer   = new GBuffer();
			m_shadowMap = new ShadowMap();

			m_default_shadow_shader   = ShaderFactory::Default3DShadowShader();
			m_default_forward_shader  = ShaderFactory::Default3DForwardShader();
			m_default_deferred_shader = ShaderFactory::Default3DDeferredShader();
			m_default_light_shader    = ShaderFactory::Default3DLightShader();

			/* 버텍스 셰이더 (기본) */
			m_VSSunnyUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::vec3) + sizeof(maths::mat4) + sizeof(maths::mat4);
			m_VSSunnyUniformBuffer = new unsigned char[m_VSSunnyUniformBufferSize];
			memset(m_VSSunnyUniformBuffer, 0, m_VSSunnyUniformBufferSize);

			m_VSSunnyUniformBufferOffsets.resize(VSSunnyUniformIndex_Size);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix]      = 0;
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix]            = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix]      + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix]           = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix]            + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition]        = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix]           + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix] = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition]        + sizeof(maths::vec3);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightViewMatrix]       = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix] + sizeof(maths::mat4);

			/* 버텍스 셰이더 (그림자) */
			m_VSSunnyShadowUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBuffer = new unsigned char[m_VSSunnyShadowUniformBufferSize];
			memset(m_VSSunnyShadowUniformBuffer, 0, m_VSSunnyShadowUniformBufferSize);

			m_VSSunnyShadowUniformBufferOffsets.resize(VSSunnyShadowUniformIndex_Size);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ProjectionMatrix]      = 0;
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ViewMatrix]            = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ProjectionMatrix]       + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix]           = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ViewMatrix]             + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix] = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix]            + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightViewMatrix]       = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix]  + sizeof(maths::mat4);
			
			/* 픽셀 셰이더 (Forward) */
			m_PSSunnyForwardUniformBufferSize = sizeof(Light) + sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyForwardUniformBuffer = new unsigned char[m_PSSunnyForwardUniformBufferSize];
			memset(m_PSSunnyForwardUniformBuffer, 0, m_PSSunnyForwardUniformBufferSize);

			m_PSSunnyForwardUniformBufferOffsets.resize(PSSunnyForwardUniformIndex_Size);
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights]     = 0;
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color]      = m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights] + sizeof(Light);
			m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_HasTexture] = m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color]  + sizeof(maths::vec4);
		
			/* 픽셀 셰이더 (Deferred) */
			m_PSSunnyDeferredUniformBufferSize = sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyDeferredUniformBuffer = new unsigned char[m_PSSunnyDeferredUniformBufferSize];
			memset(m_PSSunnyDeferredUniformBuffer, 0, m_PSSunnyDeferredUniformBufferSize);

			m_PSSunnyDeferredUniformBufferOffsets.resize(PSSunnyDeferredUniformIndex_Size);
			m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color]      = 0;
			m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_HasTexture] = m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color] + sizeof(maths::vec4);
		
			/* 픽셀 셰이더 (Light) */
			m_PSSunnyLightUniformBufferSize = sizeof(Light) + sizeof(maths::vec3);
			m_PSSunnyLightUniformBuffer = new unsigned char[m_PSSunnyLightUniformBufferSize];
			memset(m_PSSunnyLightUniformBuffer, 0, m_PSSunnyLightUniformBufferSize);

			m_PSSunnyLightUniformBufferOffsets.resize(PSSunnyLightUniformIndex_Size);
			m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights]         = 0;
			m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_CameraPosition] = m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights] + sizeof(Light);
		}

		void Renderer3D::Begin()
		{
			m_deferredCommandQueue.clear();
			m_forwardCommandQueue.clear();
			m_sunnyUniforms.clear();
		}

		void Renderer3D::BeginScene(Camera* camera)
		{
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix],       &camera->GetViewMatrix(),       sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition],   &camera->GetPosition(),         sizeof(maths::vec3));

			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ViewMatrix],       &camera->GetViewMatrix(),       sizeof(maths::mat4));

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
			// 위치와 셰이더는 그룹에 종속된다.
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

			memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Lights], lights[0], sizeof(Light));
			memcpy(m_PSSunnyLightUniformBuffer   + m_PSSunnyLightUniformBufferOffsets[PSSunnyLightUniformIndex_Lights]    , lights[0], sizeof(Light));
			
			maths::mat4 light_perspecitve = maths::mat4::Perspective(maths::SUNNY_PI / 2, 1.0f, 0.1f, 1000.0f);
			maths::mat4 light_view = maths::mat4::LookAt(lights[0]->position, maths::vec3(0.0f, 0.0f, 0.0f), maths::vec3(0.0f, 1.0f, 0.0f));

			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix], &light_perspecitve, sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightViewMatrix], &light_view, sizeof(maths::mat4));

			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix], &light_perspecitve, sizeof(maths::mat4));
			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightViewMatrix],       &light_view,        sizeof(maths::mat4));
		}

		void Renderer3D::EndScene(Camera* camera)
		{
			maths::vec3 cameraPosition = camera->GetPosition();

			// 카메라 위치와 불투명한 객체 위치의 거리에 따라 정렬
			std::sort(m_forwardCommandQueue.begin(), m_forwardCommandQueue.end(), [&cameraPosition](const RenderCommand& a, RenderCommand& b) {				
				return  cameraPosition.Distance(a.transform.GetPosition()) >
						cameraPosition.Distance(b.transform.GetPosition());
			});
		}

		void Renderer3D::End()
		{
			m_shadowMap->Bind();

			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(false);

			for (unsigned int i = 0; i < m_deferredCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_deferredCommandQueue[i];

				memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));

				m_default_shadow_shader->Bind();;

				SetSunnyShadowVSUniforms(m_default_shadow_shader);

				command.renderable3d->Render();
			}

			directx::Renderer::SetBlend(true);

			for (unsigned int i = 0; i < m_forwardCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_forwardCommandQueue[i];

				memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));

				m_default_shadow_shader->Bind();;

				SetSunnyShadowVSUniforms(m_default_shadow_shader);

				command.renderable3d->Render();
			}

			
			m_shadowMap->UnBInd();
		}

		void Renderer3D::Present()
		{

			if (DEFERRED_MODE)
			{
				DeferredBlendFalsePresentInternal();
				DeferredBlendTruePresentInternal();
				return;
			}
			

			ForwardBlendFalsePresentInternal();
			ForwardBlendTruePresentInternal();

		}

		void Renderer3D::ForwardBlendFalsePresentInternal()
		{
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(false);

			for (unsigned int i = 0; i < m_deferredCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_deferredCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
				memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_Color], &command.color, sizeof(maths::vec4));
				memcpy(m_PSSunnyForwardUniformBuffer + m_PSSunnyForwardUniformBufferOffsets[PSSunnyForwardUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

				command.shader->Bind();

				m_shadowMap->SetShadowMapTexture();

				SetSunnyVSUniforms(command.shader);
				SetSunnyForwardUniforms(command.shader);

				command.renderable3d->Render();
			}
		}

		void Renderer3D::ForwardBlendTruePresentInternal()
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

				m_shadowMap->SetShadowMapTexture();

				SetSunnyVSUniforms(command.shader);
				SetSunnyForwardUniforms(command.shader);

				command.renderable3d->Render();
			}
		}

		void Renderer3D::DeferredBlendFalsePresentInternal()
		{
			
			m_gBuffer->Bind();
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(false);

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

			m_gBuffer->UnBind();

			m_default_light_shader->Bind();

			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::POSITION);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::DIFFUSE);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::NORMAL);
			m_gBuffer->SetGBufferSampler();

			SetSunnyLightUniforms(m_default_light_shader);

			directx::Renderer::SetDepthTesting(false);

			m_gBuffer->Draw();
		}

		void Renderer3D::DeferredBlendTruePresentInternal()
		{
			m_gBuffer->Bind();
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(true);

			for (unsigned int i = 0; i < m_forwardCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_forwardCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
				memcpy(m_PSSunnyDeferredUniformBuffer + m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_Color], &command.color, sizeof(maths::vec4));
				memcpy(m_PSSunnyDeferredUniformBuffer + m_PSSunnyDeferredUniformBufferOffsets[PSSunnyDeferredUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

				command.shader = m_default_deferred_shader;
				command.shader->Bind();

				SetSunnyVSUniforms(command.shader);
				SetSunnyDeferredUniforms(command.shader);

				command.renderable3d->Render();
			}

			m_gBuffer->UnBind();

			m_default_light_shader->Bind();

			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::POSITION);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::DIFFUSE);
			m_gBuffer->SetGBufferTexture(GBuffer::TextureType::NORMAL);
			m_gBuffer->SetGBufferSampler();

			SetSunnyLightUniforms(m_default_light_shader);

			directx::Renderer::SetDepthTesting(false);

			m_gBuffer->Draw();
		}

		void Renderer3D::SetSunnyVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyUniformBuffer, m_VSSunnyUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyShadowVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyShadowUniformBuffer, m_VSSunnyShadowUniformBufferSize, 0);
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