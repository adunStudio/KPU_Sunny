#include "Renderer2D.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

#define RENDERER_MAX_SPRITES   1000
#define RENDERER_SPRITE_SIZE   RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE   RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE  RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES  16 - 1  // 0번은 쓰지 않는다.

		bool Renderer2D::s_maskEnabled = true;

		const unsigned int g_requiredSystemUniformsCount = 2;
		const std::string g_requiredSystemUniforms[g_requiredSystemUniformsCount] =
		{
			"SUNNY_ProjectionMatrix",
			"SUNNY_ViewMatrix"
		};

		const unsigned int SUNNY_ProjectionMatrixIndex = 0;
		const unsigned int SUNNY_ViewMatrixIndex       = 1;


		Renderer2D::Renderer2D(unsigned int width, unsigned int height)
		: m_indexCount(0), m_screenSize(tvec2<unsigned int>(width, height)), m_viewportSize(tvec2<unsigned int>(width, height)), m_mask(nullptr), m_camera(nullptr)
		{
			Init();
		}

		Renderer2D::Renderer2D(const maths::tvec2<unsigned int>& screenSize)
		: m_indexCount(0), m_screenSize(screenSize), m_viewportSize(screenSize), m_mask(nullptr), m_camera(nullptr)
		{
			Init();
		}

		Renderer2D::~Renderer2D()
		{
			delete m_indexBuffer;
			delete m_vertexArray;
			delete m_screenQuad;
		}

		void Renderer2D::Push(const maths::mat4& matrix, bool override)
		{
			// push는 오버라이드 되지 않는 이상 이전 값과 누적
			if (override)
				m_transformationStack.push_back(matrix);
			else
				m_transformationStack.push_back(m_transformationStack.back() * matrix);
		
			m_transformationBack = &m_transformationStack.back();
		}

		void Renderer2D::Pop()
		{
			if (m_transformationStack.size() > 1)
				m_transformationStack.pop_back();

			m_transformationBack = &m_transformationStack.back();
		}

		void Renderer2D::SetCamera(Camera* camera)
		{
			if (m_camera)
				delete m_camera;

			m_camera = camera;

			m_camera->Focus();

			memcpy(m_systemUniforms[SUNNY_ProjectionMatrixIndex].buffer.buffer + m_systemUniforms[SUNNY_ProjectionMatrixIndex].offset, &camera->GetProjectionMatrix(), sizeof(mat4));
			memcpy(m_systemUniforms[SUNNY_ViewMatrixIndex].buffer.buffer + m_systemUniforms[SUNNY_ViewMatrixIndex].offset, &camera->GetViewMatrix(), sizeof(mat4));
		}


		void Renderer2D::Begin()
		{
			m_buffer = m_vertexArray->GetBuffer()->GetPointer<Vertex2DData>();
		}

		void Renderer2D::Submit(const Renderable2D* renderable)
		{
			if (!renderable->IsVisible())
				return;

			const Rectangle& bounds = renderable->GetBounds();
			const vec3 min = bounds.GetMinimumBound();
			const vec3 max = bounds.GetMaximumBound();

			const maths::vec4& color        = renderable->GetColor();
			const std::vector<vec2>& uv     = renderable->GetUVs();
			const directx::Texture* texture = renderable->GetTexture();

			float textureSlot = 0.0f;
			if (texture)
				textureSlot = SubmitTexture(renderable->GetTexture());

			mat4 maskTransform = mat4::Identity();
			float mid = m_mask ? SubmitTexture(m_mask->texture) : 0.0f;
			float ms = 0.0f;

			if (s_maskEnabled && m_mask != nullptr)
			{
				maskTransform = mat4::Invert(m_mask->transform);
				ms = SubmitTexture(m_mask->texture);
			}

			// 시계 방향
			vec3 vertex = *m_transformationBack * min;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(max.x, min.y);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * max;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(min.x, max.y);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;


			// 반시계 방향
			/*vec3 vertex = *m_transformationBack * vec3(min.x, max.y);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * max;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(max.x, min.y);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			 vertex = *m_transformationBack * min;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = textureSlot;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;*/

			m_indexCount += 6;
		}

		void Renderer2D::End()
		{
			m_vertexArray->GetBuffer()->ReleasePointer();
		}

		void Renderer2D::Present()
		{
			// 2차원 렌더러이므로 스텐실/깊이 검사는 필요없다.
			directx::Renderer::SetDepthTesting(false);

			// 셰이더 연결
			m_shader->Bind();

			// SUNNY_ ~ 데이터
			for (unsigned int i = 0; i < m_systemUniformBuffers.size(); ++i)
				m_shader->SetVSSystemUniformBuffer(m_systemUniformBuffers[i].buffer, m_systemUniformBuffers[i].size, i);
		
			// 텍스처 연결
			for (unsigned int i = 0; i < m_textures.size(); ++i)
				m_textures[i]->Bind(i);

			m_indexBuffer->Bind(); // 인덱스 버퍼 연결
			m_vertexArray->Draw(m_indexCount);

			// 텍스쳐 연결 해제
			for (unsigned int i = 0; i < m_textures.size(); ++i)
				m_textures[i]->UnBind(i);

			m_indexCount = 0;
			m_textures.clear();
		}


		void Renderer2D::Init()
		{
			m_transformationStack.push_back(mat4::Identity());
			m_transformationBack = &m_transformationStack.back();

			m_systemUniforms.resize(g_requiredSystemUniformsCount);

			m_shader = ShaderFactory::Default2DShader();
			const directx::ShaderUniformBufferList& vssu = m_shader->GetVSSystemUniforms();

			if (vssu.size() == 0)
			{
				// Debug System
				std::cout << "Renderer2D Init() False" << std::endl;
				exit(0);
			}

			// SUNNY_~ 세팅
			for (unsigned int i = 0; i < vssu.size(); ++i)
			{
				directx::ShaderUniformBufferDeclaration* ub = vssu[i];
				UniformBuffer buffer(new unsigned char[ub->GetSize()], ub->GetSize());
				m_systemUniformBuffers.push_back(buffer);

				for (directx::ShaderUniformDeclaration* decl : ub->GetUniformDeclarations())
				{
					for (unsigned int j = 0; j < g_requiredSystemUniformsCount; ++j)
					{
						if (decl->GetName() == g_requiredSystemUniforms[j])
							m_systemUniforms[j] = BR2DSystemUniform(buffer, decl->GetOffset());
					}
				}
			}

			// TODO : 기본 카메라 Orthographcis 16, 9, 1
			SetCamera(new Camera(mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)));

			m_shader->Bind();

			directx::VertexBuffer* buffer = new directx::VertexBuffer();
			buffer->Resize(RENDERER_BUFFER_SIZE);

			// 입력레이아웃 설정
			directx::BufferLayout layout;
			layout.Push<vec3>("POSITION");
			layout.Push<vec2>("TEXCOORD");
			layout.Push<vec2>("MASKUV");
			layout.Push<float>("ID");
			layout.Push<float>("MASKID");
			layout.Push<vec4>("COLOR");
			buffer->SetLayout(layout);

			m_vertexArray = new directx::VertexArray();
			m_vertexArray->PushBuffer(buffer);

			unsigned int* indices = new unsigned int[RENDERER_INDICES_SIZE];
		
			// 사각형 인덱스 // 무조건 사각형이다. // 미리 채워 넣는다.
			int offset = 0;
			for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			m_indexBuffer = new directx::IndexBuffer(indices, RENDERER_INDICES_SIZE);		
		}

		float Renderer2D::SubmitTexture(directx::Texture* texture)
		{
			float result = 0.0f;
			bool   found = false;

			for (unsigned int i = 0; i < m_textures.size(); ++i)
			{
				// 같은 텍스처라면 추가 X
				if (m_textures[i] == texture)
				{
					result = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				// 32개 이상이라면 다시
				if (m_textures.size() >= RENDERER_MAX_TEXTURES)
				{
					End();
					Present();
					Begin();
				}
				m_textures.push_back(texture);
				result = (float)(m_textures.size());
			}

			return result;
		}

		/* 그리기 연산 */
		void Renderer2D::DrawLine(float x0, float y0, float x1, float y1, const maths::vec4& color, float thickness)
		{
			const std::vector<vec2>& uv = Renderable2D::GetDefaultUVs();
			float ts = 0.0f;
			mat4 maskTransform = mat4::Identity();
			float mid = m_mask ? SubmitTexture(m_mask->texture) : 0.0f;

			float ms = 0.0f;
			if (s_maskEnabled && m_mask != nullptr)
			{
				maskTransform = mat4::Invert(m_mask->transform);
				ms = SubmitTexture(m_mask->texture);
			}

			vec2 normal = vec2(y1 - y0, -(x1 - x0)).Normalise() * thickness;

			// 시계 방향
			vec3 vertex = *m_transformationBack * vec3(x0 + normal.x, y0 + normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x1 + normal.x, y1 + normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x1 - normal.x, y1 - normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x0 - normal.x, y0 - normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;


			// 반시계 방향
			/*vec3 vertex = *m_transformationBack * vec3(x0 - normal.x, y0 - normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x1 - normal.x, y1 - normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x1 + normal.x, y1 + normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(x0 + normal.x, y0 + normal.y, 0.0f);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;*/

			


			

			m_indexCount += 6;
		}
		void Renderer2D::DrawLine(const maths::vec2& start, const maths::vec2& end, const maths::vec4& color, float thickness)
		{
			DrawLine(start.x, start.y, end.x, end.y, color, thickness);
		}



		void Renderer2D::DrawRect(float x, float y, float width, float height, const maths::vec4& color)
		{
			DrawLine(x        , y         , x + width, y         , color); // 아래
			DrawLine(x + width, y         , x + width, y + height, color); // 우측
			DrawLine(x + width, y + height, x        , y + height, color); // 위
			DrawLine(x        , y + height, x        , y         , color); // 좌측
		}
		void Renderer2D::DrawRect(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color)
		{
			DrawRect(position.x, position.y, size.x, size.y, color);
		}
		void Renderer2D::DrawRect(const maths::Rectangle& rectangle, const maths::vec4& color)
		{
			DrawRect(rectangle.GetMinimumBound(), rectangle.size * 2.0f, color);
		}



		void Renderer2D::FillRect(float x, float y, float width, float height, const maths::vec4& color)
		{
			vec3 position(x, y, 0.0f);
			vec2 size(width, height);
			const std::vector<vec2>& uv = Renderable2D::GetDefaultUVs();
			float ts = 0.0f;
			mat4 maskTransform = mat4::Identity();
			float mid = m_mask ? SubmitTexture(m_mask->texture) : 0.0f;

			float ms = 0.0f;
			if (s_maskEnabled && m_mask != nullptr)
			{
				maskTransform = mat4::Invert(m_mask->transform);
				ms = SubmitTexture(m_mask->texture);
			}

			// 시계 방향
			vec3 vertex = *m_transformationBack * position;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(position.x + size.x, position.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(position.x + size.x, position.y + size.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(position.x, position.y + size.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			// 반 시계 방향
			/*vec3 vertex = *m_transformationBack * vec3(position.x, position.y + size.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[3];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(position.x + size.x, position.y + size.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[2];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * vec3(position.x + size.x, position.y, position.z);
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[1];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;

			vertex = *m_transformationBack * position;
			m_buffer->vertex = vertex;
			m_buffer->uv = uv[0];
			m_buffer->mask_uv = maskTransform * vertex;
			m_buffer->tid = ts;
			m_buffer->mid = ms;
			m_buffer->color = color;
			m_buffer++;*/

			m_indexCount += 6;
		}
		void Renderer2D::FillRect(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color)
		{
			FillRect(position.x, position.y, size.x, size.y, color);
		}
		void Renderer2D::FillRect(const maths::Rectangle& rectangle, const maths::vec4& color)
		{
			FillRect(rectangle.GetMinimumBound(), rectangle.size * 2.0f, color);
		}
	}
}