#pragma once

#include "../../sunny.h"

#include "../renderables/Renderable2D.h"
#include "../Mask.h"
#include "../cameras/Camera.h"
#include "../shaders/ShaderFactory.h"

#include "../../directx/Renderer.h"
#include "../../directx/VertexBuffer.h"
#include "../../directx/VertexArray.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/BufferLayout.h"

namespace sunny
{
	namespace graphics
	{
		struct UniformBuffer
		{
			unsigned char* buffer;
			unsigned int size;

			UniformBuffer() {};
			UniformBuffer(unsigned char* buffer, unsigned int size)
			: buffer(buffer), size(size)
			{
				memset(buffer, 0, size);
			}
		};

		struct BR2DSystemUniform
		{
			UniformBuffer buffer;
			unsigned int offset;

			BR2DSystemUniform() {};
			BR2DSystemUniform(const UniformBuffer& buffer, unsigned int offset)
			: buffer(buffer), offset(offset)
			{}
		};

		class Renderer2D
		{
		private:
			static bool s_maskEnabled;

		private:
			std::vector<maths::mat4> m_transformationStack;
			const maths::mat4* m_transformationBack;
			const Mask* m_mask;
			
			directx::Shader* m_shader;
			std::vector<BR2DSystemUniform> m_systemUniforms;
			std::vector<UniformBuffer>     m_systemUniformBuffers;

			directx::VertexArray* m_vertexArray;
			directx::IndexBuffer* m_indexBuffer;
			directx::IndexBuffer* m_lineIBO;

			unsigned int m_indexCount, m_lineIndexCount;

			Vertex2DData* m_buffer;

			std::vector<directx::Texture*> m_textures;
			
			maths::tvec2<unsigned int> m_viewportSize, m_screenSize;

			directx::VertexArray* m_screenQuad;

			Camera* m_camera;

		public:
			Renderer2D(unsigned int width, unsigned int height);
			Renderer2D(const maths::tvec2<unsigned int>& screenSize);
			~Renderer2D();

			void SetCamera(Camera* camera);

			void Push(const maths::mat4& matrix, bool override = false);
			void Pop();

			// 
			void Begin();
			void Submit(const Renderable2D* renderable);
			void End();
			void Present();

			// 그리기 연산
			void DrawLine(float x0, float y0, float x1, float y1, const maths::vec4& color = maths::vec4(1, 1, 1, 1), float thickness = 0.02f);
			void DrawLine(const maths::vec2& start, const maths::vec2& end, const maths::vec4& color = maths::vec4(1, 1, 1, 1), float thickness = 0.02f);
			
			void DrawRect(float x, float y, float width, float height, const maths::vec4& color = maths::vec4(1, 1, 1, 1));
			void DrawRect(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color = maths::vec4(1, 1, 1, 1));
			void DrawRect(const maths::Rectangle& rectangle, const maths::vec4& color = maths::vec4(1, 1, 1, 1));

			void FillRect(float x, float y, float width, float height, const maths::vec4& color = maths::vec4(1, 1, 1, 1));
			void FillRect(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color = maths::vec4(1, 1, 1, 1));
			void FillRect(const maths::Rectangle& rectangle, const maths::vec4& color = maths::vec4(1, 1, 1, 1));

			inline void SetMask(const Mask* mask) { m_mask = mask; }

			inline void SetScreenSize(const maths::tvec2<unsigned int>& size) { m_screenSize = size; }
			inline const maths::tvec2<unsigned int>& GetScreenSize() const { return m_screenSize; }

			inline void SetViewportSize(const maths::tvec2<unsigned int>& size) { m_viewportSize = size; }
			inline const maths::tvec2<unsigned int>& GetViewportSize() const { return m_viewportSize; }

		private:
			void Init();
			float SubmitTexture(directx::Texture* texture);
		};
	}
}