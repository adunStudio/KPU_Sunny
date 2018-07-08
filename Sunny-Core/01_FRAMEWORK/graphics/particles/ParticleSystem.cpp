#include "ParticleSystem.h"
#include "../shaders/ShaderFactory.h"

namespace sunny
{
	namespace graphics
	{
		ParticleSystem::ParticleSystem(directx::Texture* texture)
		: m_texture(texture), m_currentParticleCount(0), m_maxParticleCount(100), m_accumulatedTime(0.0f)
		{
			m_particleList     = new Particle[m_maxParticleCount];
			m_particleVertices = new ParticleVertex[m_maxParticleCount];

			m_vertexCount = m_maxParticleCount * 6;
			m_indexCount = m_vertexCount;

			unsigned int* indices = new unsigned int[m_indexCount];

			memset(m_particleVertices, 0, sizeof(ParticleVertex) * m_vertexCount);

			for (int i = 0; i < m_maxParticleCount; ++i)
			{
				m_particleList[i].active = false;
				indices[i] = i;
			}

	

			ShaderFactory::DefaultParticleShader()->Bind();

			m_vertexBuffer = new directx::VertexBuffer();
			m_vertexBuffer->SetData(m_vertexCount * sizeof(ParticleVertex), &m_particleVertices[0]);

			directx::BufferLayout layout;
			layout.Push<maths::vec3>("POSITION");
			layout.Push<maths::vec2>("TEXCOORD");
			layout.Push<maths::vec3>("COLOR");
			m_vertexBuffer->SetLayout(layout);

			directx::VertexArray* va = new directx::VertexArray();
			va->PushBuffer(m_vertexBuffer);

			directx::IndexBuffer* ib = new directx::IndexBuffer(&indices[0], m_indexCount);

			delete[] indices;
		}

		ParticleSystem::~ParticleSystem()
		{
			delete[] m_particleList;
			delete[] m_particleVertices;
			delete m_vertexArray;
			delete m_vertexBuffer;
			delete m_indexBuffer;
		}

		void ParticleSystem::Update(const utils::Timestep& ts)
		{
			KillParticles();

			EmitParticles(ts);

			for (int i = 0; i < m_currentParticleCount; ++i)
			{
				m_particleList[i].position.y = m_particleList[i].position.y - (m_particleList[i].velocity * ts.GetMillis() * 0.001f);
			}

			UpdateBuffers();
		}

		void ParticleSystem::Render()
		{
			m_texture->Bind();
			m_indexBuffer->Bind();
			m_vertexArray->Draw(m_indexBuffer->GetCount());
			m_texture->UnBind();
		}

		void  ParticleSystem::KillParticles()
		{
			for (int i = 0; i < m_maxParticleCount; ++i)
			{
				if (m_particleList[i].active && (m_particleList[i].position.y < -3.0f))
				{
					m_particleList[i].active = false;
					m_currentParticleCount--;

					for (int j = i; j < m_maxParticleCount - 1; ++j)
					{
						m_particleList[i].position = m_particleList[i + 1].position;
						m_particleList[i].color    = m_particleList[i + 1].color;
						m_particleList[i].velocity = m_particleList[i + 1].velocity;
						m_particleList[i].active   = m_particleList[i + 1].active;
					}
				}
			}
		}

		void  ParticleSystem::EmitParticles(const utils::Timestep& ts)
		{
			m_accumulatedTime += ts.GetMillis();

			bool emitParticle = false;

			if (m_accumulatedTime > (1000.0f / m_particlePerSecond))
			{
				m_accumulatedTime = 0.0f;
				emitParticle = true;
			}

			if (emitParticle && (m_currentParticleCount < (m_maxParticleCount - 1)))
			{
				m_currentParticleCount++;

				float positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 0.5f;
				float positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 0.1f;
				float positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * 2.0f;

				float velocity = 1.0f + (((float)rand() - (float)rand()) / RAND_MAX) * 0.2f;
			
				float red   = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
				float green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
				float blue  = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

				int index = 0;
				bool found = false;

				while (!found)
				{
					if ((m_particleList[index].active == false) || m_particleList[index].position.z < positionZ)
						found = true;
					else
						index++;
				}

				int i = m_currentParticleCount;
				int j = i - 1;

				while (i != index)
				{
					m_particleList[i].position = m_particleList[j].position;
					m_particleList[i].color    = m_particleList[j].color;
					m_particleList[i].velocity = m_particleList[j].velocity;
					m_particleList[i].active   = m_particleList[j].active;

					i--;
					j--;
				}

				m_particleList[index].position = maths::vec3(positionX, positionY, positionZ);
				m_particleList[index].color    = maths::vec4(red, green, blue, 1.0f);
				m_particleList[index].velocity = velocity;
				m_particleList[index].active = true;
			}
		}

		void  ParticleSystem::UpdateBuffers()
		{
			memset(m_particleVertices, 0, sizeof(ParticleVertex) * m_vertexCount);

			int index = 0;

			for (int i = 0; i < m_currentParticleCount; ++i)
			{
				// 왼쪽 아래
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(0, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 왼쪽 위
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(0, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 오른쪽 아래
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(1, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 오른쪽 아래
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(1, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 왼쪽 위
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(0, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;
			
				// 오른쪽 위
				m_particleVertices[index].position = m_particleList[i].position;
				m_particleVertices[index].uv       = maths::vec2(1, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;
			}

			m_vertexBuffer->SetData(m_vertexCount * sizeof(ParticleVertex), &m_particleVertices[0]);
		}
	}
}