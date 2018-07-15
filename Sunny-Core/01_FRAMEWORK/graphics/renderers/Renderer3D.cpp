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
			VSSunnyUniformIndex_LightProjectionMatrix = 3,
			VSSunnyUniformIndex_LightViewMatrix       = 4,
			VSSunnyUniformIndex_CameraPosition        = 5,
			VSSunnyUniformIndex_Size
		};

		enum PSSunnyUniformIndices : int
		{
			PSSunnyUniformIndex_Lights                = 0,
			PSSunnyUniformIndex_Color                 = 1,
			PSSunnyUniformIndex_HasTexture            = 2,
			PSSunnyUniformIndex_Size
		};

		enum VSSunnyShadowUniformIndices : int
		{
			VSSunnyShadowUniformIndex_ModelMatrix           = 0,
			VSSunnyShadowUniformIndex_LightProjectionMatrix = 1,
			VSSunnyShadowUniformIndex_LightViewMatrix       = 2,
			VSSunnyShadowUniformIndex_Size
		};

		enum PSSunnyGeometryUniformIndices : int
		{
			PSSunnyGeometryUniformIndex_Color               = 0,
			PSSunnyGeometryUniformIndex_ID                  = 1,
			PSSunnyGeometryUniformIndex_HasTexture          = 2,
			PSSunnyGeometryUniformIndex_Size
		};

		enum VSSunnyParticleUniformIndices : int
		{
			VSSunnyParticleUniformIndex_ProjectionMatrix    = 0,
			VSSunnyParticleUniformIndex_ViewMatrix          = 1,
			VSSunnyParticleUniformIndex_ModelMatrix         = 2,
			VSSunnyParticleUniformIndex_Size
		};

		Renderer3D::Renderer3D()
		: m_skybox(nullptr)
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
			m_particles.reserve(100);
			m_renderCommandQueue.reserve(1000);
			m_staticCommandQueue.reserve(200);

			m_gBuffer     = new GBuffer();

			m_lightCamera = new LightCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f));

			m_default_shadow_shader   = ShaderFactory::Default3DShadowShader();
			m_default_forward_shader  = ShaderFactory::Default3DForwardShader();
			m_default_geometry_shader = ShaderFactory::Default3DGeometryShader();
			m_default_outline_shader  = ShaderFactory::Default3DOutLineShader();
			m_default_particle_shader = ShaderFactory::DefaultParticleShader();


			/* 버텍스 셰이더 (기본) */
			m_VSSunnyUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::vec3);
			m_VSSunnyUniformBuffer     = new unsigned char[m_VSSunnyUniformBufferSize];
			memset(m_VSSunnyUniformBuffer, 0, m_VSSunnyUniformBufferSize);

			m_VSSunnyUniformBufferOffsets.resize(VSSunnyUniformIndex_Size);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix]      = 0;
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix]            = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix]      + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix]           = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix]            + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix] = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix]           + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightViewMatrix]	     = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix] + sizeof(maths::mat4);
			m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition]        = m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightViewMatrix]       + sizeof(maths::mat4);
			
			/* 픽셀 셰이더 (기본) */
			m_PSSunnyUniformBufferSize = sizeof(Light) + sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyUniformBuffer     = new unsigned char[m_PSSunnyUniformBufferSize];
			memset(m_PSSunnyUniformBuffer, 0, m_PSSunnyUniformBufferSize);

			m_PSSunnyUniformBufferOffsets.resize(PSSunnyUniformIndex_Size);
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights]                = 0;
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color]                 = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights]                + sizeof(Light);
			m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture]            = m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color]                 + sizeof(maths::vec4);


			/* 버텍스 셰이더 (그림자) */
			m_VSSunnyShadowUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBuffer     = new unsigned char[m_VSSunnyShadowUniformBufferSize];
			memset(m_VSSunnyShadowUniformBuffer, 0, m_VSSunnyShadowUniformBufferSize);

			m_VSSunnyShadowUniformBufferOffsets.resize(VSSunnyShadowUniformIndex_Size);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix]           = 0;
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix] = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix]            + sizeof(maths::mat4);
			m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightViewMatrix]       = m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix]  + sizeof(maths::mat4);
		
		
			/* 픽셀 셰이더 (Geometry) */
			m_PSSunnyGeometryUniformBufferSize = sizeof(maths::vec4) + sizeof(maths::vec4) + sizeof(float);
			m_PSSunnyGeometryUniformBuffer     = new unsigned char[m_PSSunnyGeometryUniformBufferSize];
			memset(m_PSSunnyGeometryUniformBuffer, 0, m_PSSunnyGeometryUniformBufferSize);

			m_PSSunnyGeometryUniformBufferOffsets.resize(PSSunnyGeometryUniformIndex_Size);
			m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_Color]      = 0;
			m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_ID]         = m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_Color] + sizeof(maths::vec4);
			m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_HasTexture] = m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_ID] + sizeof(maths::vec4);


			/* 버텍스 셰이더 (파티클) */
			m_VSSunnyParticleUniformBufferSize = sizeof(maths::mat4) + sizeof(maths::mat4) + sizeof(maths::mat4);
			m_VSSunnyParticleUniformBuffer     = new unsigned char[m_VSSunnyParticleUniformBufferSize];
			memset(m_VSSunnyParticleUniformBuffer, 0, m_VSSunnyParticleUniformBufferSize);

			m_VSSunnyParticleUniformBufferOffsets.resize(VSSunnyParticleUniformIndex_Size);
			m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ProjectionMatrix] = 0;
			m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ViewMatrix]       = m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ProjectionMatrix] + sizeof(maths::mat4);
			m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ModelMatrix]      = m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ViewMatrix] + sizeof(maths::mat4);
		}

		void Renderer3D::Begin()
		{
			m_renderCommandQueue.clear();
			m_sunnyUniforms.clear();
			m_particles.clear();
		}

		void Renderer3D::BeginScene(Camera* camera)
		{
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ViewMatrix],       &camera->GetViewMatrix(),       sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_CameraPosition],   &camera->GetPosition(),         sizeof(maths::vec3));

			memcpy(m_VSSunnyParticleUniformBuffer + m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(maths::mat4));
			

			// 빌보드
			maths::mat4 matView = maths::mat4(camera->GetViewMatrix());
			matView.SetPosition(maths::vec3(0));
			matView = matView.Invert();

		
			memcpy(m_VSSunnyParticleUniformBuffer + m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ViewMatrix], &camera->GetViewMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyParticleUniformBuffer + m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ModelMatrix], &(matView), sizeof(maths::mat4));
		}

		void Renderer3D::Submit(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);
		}

		void Renderer3D::Submit(const RenderCommand& command)
		{
			m_renderCommandQueue.push_back(command);
		}

		void Renderer3D::SubmitStatic(const RenderCommand& command)
		{
			m_staticCommandQueue.push_back(command);
		}

		void Renderer3D::SubmitParticle(ParticleSystem* particle)
		{
			m_particles.push_back(particle);
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

		void Renderer3D::SubmitStatic3D(Renderable3D* renderable)
		{

			RenderCommand command;

			command.renderable3d = renderable;
			command.color = renderable->GetColor();
			command.transform = renderable->GetComponent<component::TransformComponent>()->GetTransform();
			command.hasTexture = renderable->GetHasTexture();
			command.shader = renderable->GetShader() ? renderable->GetShader() : m_default_forward_shader;

			SubmitStatic(command);
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

			lights[0]->position = m_lightCamera->GetPosition().Normalize();

			memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Lights], lights[0], sizeof(Light));
		}

		void Renderer3D::EndScene(Camera* camera)
		{
			maths::vec3 cameraPosition = camera->GetPosition();

			// 카메라 위치와  객체 위치의 거리에 따라 정렬
			std::sort(m_renderCommandQueue.begin(), m_renderCommandQueue.end(), [&cameraPosition](const RenderCommand& a, RenderCommand& b) {
				return  cameraPosition.Distance(a.transform.GetPosition()) <
					cameraPosition.Distance(b.transform.GetPosition());
			});
		}

		void Renderer3D::End()
		{
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightProjectionMatrix],                   &m_lightCamera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_LightViewMatrix],                         &m_lightCamera->GetViewMatrix(),       sizeof(maths::mat4));

			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightProjectionMatrix], &m_lightCamera->GetProjectionMatrix(), sizeof(maths::mat4));
			memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_LightViewMatrix],       &m_lightCamera->GetViewMatrix(),       sizeof(maths::mat4));
			
			MakeShadowGeometryBuffer();
		}

		void Renderer3D::Present()
		{		
			SkyboxPresentInternal();

			ForwardPresentInternal();

			ParticlePresentInternal();
		}

		void Renderer3D::MakeShadowGeometryBuffer()
		{
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(true);
		
			// 그림자 생성
			{
				m_gBuffer->Bind(GBufferType::SHADOWMAP);
				m_default_shadow_shader->Bind();

				for (unsigned int i = 0; i < m_staticCommandQueue.size(); ++i)
				{
					RenderCommand& command = m_staticCommandQueue[i];

					memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));

					SetSunnyShadowVSUniforms(m_default_shadow_shader);

					command.renderable3d->Render();
				}

				for (unsigned int i = 0; i < m_renderCommandQueue.size(); ++i)
				{
					RenderCommand& command = m_renderCommandQueue[i];

					memcpy(m_VSSunnyShadowUniformBuffer + m_VSSunnyShadowUniformBufferOffsets[VSSunnyShadowUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));

					SetSunnyShadowVSUniforms(m_default_shadow_shader);

					command.renderable3d->Render();
				}
			}
			

			// 지오메트리 생성{
			{
				m_gBuffer->Bind(GBufferType::DEFERRED);
				m_default_geometry_shader->Bind();

				for (unsigned int i = 0; i < m_staticCommandQueue.size(); ++i)
				{
					RenderCommand& command = m_staticCommandQueue[i];

					memcpy(m_VSSunnyUniformBuffer         + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_Color], &command.color, sizeof(maths::vec4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_ID], &command.renderable3d->GetIDColor(), sizeof(maths::vec4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

					SetSunnyVSUniforms(m_default_geometry_shader);
					SetSunnyGeometryPSUniforms(m_default_geometry_shader);

					command.renderable3d->Render();
				}

				for (unsigned int i = 0; i < m_renderCommandQueue.size(); ++i)
				{
					RenderCommand& command = m_renderCommandQueue[i];

					memcpy(m_VSSunnyUniformBuffer         + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform, sizeof(maths::mat4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_Color], &command.color, sizeof(maths::vec4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_ID], &command.renderable3d->GetIDColor(), sizeof(maths::vec4));
					memcpy(m_PSSunnyGeometryUniformBuffer + m_PSSunnyGeometryUniformBufferOffsets[PSSunnyGeometryUniformIndex_HasTexture], &command.hasTexture, sizeof(float));

					SetSunnyVSUniforms        (m_default_geometry_shader);
					SetSunnyGeometryPSUniforms(m_default_geometry_shader);

					command.renderable3d->Render();
				}
			}
			

			m_gBuffer->UnBind();
		}

		void Renderer3D::ForwardPresentInternal()
		{
			directx::Renderer::SetDepthTesting(true);
			directx::Renderer::SetBlend(true);

			for (unsigned int i = 0; i < m_staticCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_staticCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform,  sizeof(maths::mat4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color],       &command.color,      sizeof(maths::vec4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture],  &command.hasTexture, sizeof(float));

				command.shader->Bind();

				SetSunnyVSUniforms(command.shader);
				SetSunnyPSUniforms(command.shader);
				m_gBuffer->SetGBuffer(GBufferType::SHADOWMAP);

				command.renderable3d->Render();
			}

			for (unsigned int i = 0; i < m_renderCommandQueue.size(); ++i)
			{
				RenderCommand& command = m_renderCommandQueue[i];

				memcpy(m_VSSunnyUniformBuffer + m_VSSunnyUniformBufferOffsets[VSSunnyUniformIndex_ModelMatrix], &command.transform,  sizeof(maths::mat4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_Color],       &command.color,      sizeof(maths::vec4));
				memcpy(m_PSSunnyUniformBuffer + m_PSSunnyUniformBufferOffsets[PSSunnyUniformIndex_HasTexture],  &command.hasTexture, sizeof(float));

				command.shader->Bind();

				SetSunnyVSUniforms(command.shader);
				SetSunnyPSUniforms(command.shader);
				m_gBuffer->SetGBuffer(GBufferType::SHADOWMAP);

				command.renderable3d->Render();
			}

			// 아웃라인 그리기
			//m_default_outline_shader->Bind();

			//SetSunnyPSUniforms(m_default_outline_shader);

			//m_gBuffer->SetGBuffer(GBufferType::DEFERRED);

			//m_gBuffer->Draw();
		}

		void Renderer3D::SkyboxPresentInternal()
		{
			if (!m_skybox) return;

			directx::Renderer::SetDepthTesting(false);

			directx::Shader* shader = m_skybox->GetShader();

			shader->Bind();

			m_skybox->Render();
		}

		void Renderer3D::ParticlePresentInternal()
		{
			directx::Renderer::SetDepthTesting(true);
			//directx::Renderer::SetBlend(true);

			for (ParticleSystem* particle : m_particles)
			{
				m_default_particle_shader->Bind();
				//memcpy(m_VSSunnyParticleUniformBuffer + m_VSSunnyParticleUniformBufferOffsets[VSSunnyParticleUniformIndex_ModelMatrix], &(maths::mat4::Identity()), sizeof(maths::mat4));

				SetSunnyParticleVSUniforms(m_default_particle_shader);
				
				particle->Render();
			}
		}

		void Renderer3D::SetSunnyVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyUniformBuffer, m_VSSunnyUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyPSUniforms(directx::Shader* shader)
		{
			shader->SetPSSystemUniformBuffer(m_PSSunnyUniformBuffer, m_PSSunnyUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyShadowVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyShadowUniformBuffer, m_VSSunnyShadowUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyGeometryPSUniforms(directx::Shader* shader)
		{
			shader->SetPSSystemUniformBuffer(m_PSSunnyGeometryUniformBuffer, m_PSSunnyGeometryUniformBufferSize, 0);
		}

		void Renderer3D::SetSunnyParticleVSUniforms(directx::Shader* shader)
		{
			shader->SetVSSystemUniformBuffer(m_VSSunnyParticleUniformBuffer, m_VSSunnyParticleUniformBufferSize, 0);
		}
	}
}