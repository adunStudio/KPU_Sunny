#pragma once

#include "../../include.h"
#include "../renderables/Renderable3D.h"
#include "../groups/Group3D.h"
#include "../cameras/Camera.h"
#include "../cameras/LightCamera.h"
#include "../LightSetup.h"
#include "../shaders/ShaderFactory.h"
#include "../GBuffer.h"
#include "../../directx/DebugBuffer.h"

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
		private:
			GBuffer*   m_gBuffer;           // ���۵�, �׸���

			LightCamera* m_lightCamera;

			directx::Shader* m_default_forward_shader;          // ������     ���̴�
			directx::Shader* m_default_geometry_shader;         // ������Ʈ�� ���̴� 
			directx::Shader* m_default_shadow_shader;           // �׸���     ���̴� 
			directx::Shader* m_default_debug_shader;            // �����     ���̴�
			directx::Shader* m_default_outline_shader;          // �ܰ���     ���̴�

			std::vector<Renderable3D*> m_renderables;

			std::vector<RenderCommand> m_renderCommandQueue;   

			std::vector<RendererUniform> m_sunnyUniforms;

			unsigned int m_screenBufferWidth, m_screenBufferHeight;

			unsigned char* m_VSSunnyUniformBuffer;                                  // �⺻   ���ؽ� ���̴� CBuffer                    
			unsigned int   m_VSSunnyUniformBufferSize;

			unsigned char* m_VSSunnyShadowUniformBuffer;                            // �׸��� ���ؽ� ���̴� CBuffer                    
			unsigned int   m_VSSunnyShadowUniformBufferSize;

			unsigned char* m_PSSunnyForwardUniformBuffer;                           // ������ �ȼ� ���̴� CBuffer
			unsigned int   m_PSSunnyForwardUniformBufferSize;
			
			unsigned char* m_PSSunnyGeometryUniformBuffer;                          // ������Ʈ�� �ȼ� ���̴� CBuffer
			unsigned int   m_PSSunnyGeometryUniformBufferSize;
			;

			std::vector<unsigned int> m_VSSunnyUniformBufferOffsets;                   // �⺻   ���ؽ� ���̴� CBuffer ������
			std::vector<unsigned int> m_VSSunnyShadowUniformBufferOffsets;             // �׸��� ���ؽ� ���̴� CBuffer ������
			std::vector<unsigned int> m_PSSunnyForwardUniformBufferOffsets;            // ������ �ȼ� ���̴� CBuffer ������
			std::vector<unsigned int> m_PSSunnyGeometryUniformBufferOffsets;           // ������Ʈ�� �ȼ� ���̴� CBuffer ������

			

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
			void MakeShadowGeometryBuffer();

			void ForwardPresentInternal();

			void SetSunnyVSUniforms(directx::Shader* shader);
			void SetSunnyShadowVSUniforms(directx::Shader* shader);
			void SetSunnyForwardUniforms(directx::Shader* shader);
			void SetSunnyGeometryUniforms(directx::Shader* shader);
		};
	}
}