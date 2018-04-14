#pragma once

#include "../../sunny.h"
#include "../renderables/Renderable3D.h"
#include "../groups/Group3D.h"
#include "../cameras/Camera.h"
#include "../LightSetup.h"
#include "../shaders/ShaderFactory.h"
#include "../GBuffer.h"
#include "../ShadowMap.h"

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
			Renderable3D* renderable3d;
			maths::mat4 transform;
			maths::vec4 color;
			float hasTexture;
			directx::Shader* shader;
			std::vector<RendererUniform> uniforms;
		};

		class Renderer3D
		{
		public:
			static bool DEFERRED_MODE;

		private:
			GBuffer*   m_gBuffer;           // 디퍼드 렌더링
			ShadowMap* m_shadowMap;         // 그림자 렌더링

			directx::Shader* m_default_forward_shader;          // 포워드 셰이더
			directx::Shader* m_default_deferred_shader;         // 디퍼드 셰이더 (지오메트리)
			directx::Shader* m_default_light_shader;            // 라이트 셰이더 (디퍼드 셰이더 이후)
			directx::Shader* m_default_shadow_shader;           // 그림자 셰이더 (포워드 셰이더 이전)

			std::vector<Renderable3D*> m_renderables;

			std::vector<RenderCommand> m_forwardCommandQueue;               // 반투명
			std::vector<RenderCommand> m_deferredCommandQueue;              // 불투명

			std::vector<RendererUniform> m_sunnyUniforms;

			unsigned int m_screenBufferWidth, m_screenBufferHeight;

			unsigned char* m_VSSunnyUniformBuffer;                                  // 기본   버텍스 셰이더 CBuffer                    
			unsigned int   m_VSSunnyUniformBufferSize;

			unsigned char* m_VSSunnyShadowUniformBuffer;                            // 그림자 버텍스 셰이더 CBuffer                    
			unsigned int   m_VSSunnyShadowUniformBufferSize;

			unsigned char* m_PSSunnyForwardUniformBuffer;                           // 포워드 픽셀 셰이더 CBuffer
			unsigned int   m_PSSunnyForwardUniformBufferSize;
			
			unsigned char* m_PSSunnyDeferredUniformBuffer;                          // 디퍼드 픽셀 셰이더 CBuffer
			unsigned int   m_PSSunnyDeferredUniformBufferSize;
			
			unsigned char* m_PSSunnyLightUniformBuffer;                             // 라이트 픽셀 셰이더 CBuffe
			unsigned int   m_PSSunnyLightUniformBufferSize;

			std::vector<unsigned int> m_VSSunnyUniformBufferOffsets;                   // 기본   버텍스 셰이더 CBuffer 오프셋
			std::vector<unsigned int> m_VSSunnyShadowUniformBufferOffsets;             // 그림자 버텍스 셰이더 CBuffer 오프셋
		
			std::vector<unsigned int> m_PSSunnyForwardUniformBufferOffsets;            // 포워드 픽셀 셰이더 CBuffer 오프셋
			std::vector<unsigned int> m_PSSunnyDeferredUniformBufferOffsets;           // 디퍼드 픽셀 셰이더 CBuffer 오프셋
			std::vector<unsigned int> m_PSSunnyLightUniformBufferOffsets;              // 라이트 픽셀 셰이더 CBuffer 오프셋

			

		public:
			Renderer3D();
			Renderer3D(unsigned with, unsigned height);
			~Renderer3D() {};

			void Init();
			void Begin();
			void BeginScene(Camera* camera);
			void Submit(Renderable3D* renderable);
			void Submit(const RenderCommand& command);
			void SubmitRenderable3D(Renderable3D* renderable);
			void SubmitGroup3D(Group3D* group3d);
			void SubmitLight(const LightSetup& lightSetup);
			void EndScene(Camera* camera);
			void End();
			void Present();

			inline const void SetScreenBufferSize(unsigned int width, unsigned int height) { m_screenBufferWidth = width; m_screenBufferHeight = height; }
		
		private:
			void ForwardBlendFalsePresentInternal();
			void ForwardBlendTruePresentInternal();
			void DeferredBlendFalsePresentInternal();
			void DeferredBlendTruePresentInternal();

			void SetSunnyVSUniforms(directx::Shader* shader);
			void SetSunnyShadowVSUniforms(directx::Shader* shader);
			void SetSunnyForwardUniforms(directx::Shader* shader);
			void SetSunnyDeferredUniforms(directx::Shader* shader);
			void SetSunnyLightUniforms(directx::Shader* shader);
		};
	}
}