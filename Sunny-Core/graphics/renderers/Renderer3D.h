#pragma once

#include "../../sunny.h"
#include "../renderables/Renderable3D.h"
#include "../Entity.h"
#include "../cameras/Camera.h"
#include "../LightSetup.h"
#include "../../directx/Shader.h"
#include "../shaders/ShaderFactory.h"


namespace sunny
{
	namespace graphics
	{
		struct RendererUniform
		{
			std::string uniform;
			unsigned char* value;
		};

		struct RenderCommand
		{
			Entity* entity;
			maths::mat4 transform;
			maths::vec4 color;
			float hasTexture;
			directx::Shader* shader;
			std::vector<RendererUniform> uniforms;
		};

		class Renderer3D
		{
		private:
			directx::Shader* m_default_shader;

			std::vector<RenderCommand> m_commandQueue;

			std::vector<RendererUniform> m_sunnyUniforms;

			unsigned int m_screenBufferWidth, m_screenBufferHeight;

			unsigned char* m_VSSunnyUniformBuffer;
			unsigned int   m_VSSunnyUniformBufferSize;
			unsigned char* m_PSSunnyUniformBuffer;
			unsigned int   m_PSSunnyUniformBufferSize;

			std::vector<unsigned int> m_VSSunnyUniformBufferOffsets;
			std::vector<unsigned int> m_PSSunnyUniformBufferOffsets;

			std::vector<Renderable3D*> m_renderables;

		public:
			Renderer3D();
			Renderer3D(unsigned with, unsigned height);
			~Renderer3D() {};

			void Init();
			void Begin();
			void BeginScene(Camera* camera);
			void Submit(Renderable3D* renderable);
			void Submit(const RenderCommand& command);
			void SubmitEntity(Entity* entity);
			void SubmitLight(const LightSetup& lightSetup);
			void EndScene();
			void End();
			void Present();

			inline const void SetScreenBufferSize(unsigned int width, unsigned int height) { m_screenBufferWidth = width; m_screenBufferHeight = height; }
		
		private:
			void SetSunnyUniforms(directx::Shader* shader);
		};
	}
}