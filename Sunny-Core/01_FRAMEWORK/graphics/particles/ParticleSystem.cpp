#include "ParticleSystem.h"
#include "../shaders/ShaderFactory.h"

namespace sunny
{
	namespace graphics
	{
		ParticleSystem::ParticleSystem(directx::Texture* texture)
		: m_texture(texture), m_currentParticleCount(0), m_maxParticleCount(3), m_accumulatedTime(0.0f), m_visible(true)
		{
			m_vertexCount = m_maxParticleCount * 6;
			m_indexCount = m_vertexCount;

			m_particleList        = new Particle[m_maxParticleCount];
			m_particleVertices    = new ParticleVertex[m_vertexCount];

			unsigned int* indices = new unsigned int[m_indexCount];

			memset(m_particleVertices, 0, sizeof(ParticleVertex) * m_vertexCount);

			for (int i = 0; i < m_maxParticleCount; ++i)
			{
				m_particleList[i].active = false;
			}

			for (int i = 0; i < m_indexCount; ++i)
			{
				indices[i] = i;
			}


			ShaderFactory::DefaultParticleShader()->Bind();

			m_vertexBuffer = new directx::VertexBuffer();
			m_vertexBuffer->SetData(m_vertexCount * sizeof(ParticleVertex), &m_particleVertices[0]);
	

			directx::BufferLayout layout;
			layout.Push<maths::vec3>("POSITION");
			layout.Push<maths::vec3>("COLOR");
			layout.Push<maths::vec2>("TEXCOORD");
			m_vertexBuffer->SetLayout(layout);

			m_vertexArray = new directx::VertexArray();
			m_vertexArray->PushBuffer(m_vertexBuffer);

			m_indexBuffer = new directx::IndexBuffer(&indices[0], m_indexCount);

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

			/**for (int i = 0; i < m_currentParticleCount; ++i)
			{
				m_particleList[i].position.y = m_particleList[i].position.y - (m_particleList[i].velocity * ts.GetMillis() * 0.001f);
			}**/

			UpdateBuffers();
		}

		void ParticleSystem::Render()
		{
		
			m_texture->Bind();
			m_indexBuffer->Bind();
			//m_vertexArray->Draw(m_indexBuffer->GetCount());
			m_vertexArray->Draw(m_currentParticleCount * 6);
			m_texture->UnBind();
		}

		void  ParticleSystem::KillParticles()
		{
			return;
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
				//m_currentParticleCount++;

				float positionX = 0;// (((float)rand() - (float)rand()) / RAND_MAX) * 100;// 0.5f;
				float positionY = 0;// (((float)rand() - (float)rand()) / RAND_MAX) * 100;// 0.1f;
				float positionZ = 0;// (((float)rand() - (float)rand()) / RAND_MAX) * 100;// 2.0f;

				float velocity = 1.0f + (((float)rand() - (float)rand()) / RAND_MAX) * 0.2f;
			
				float red = 1;// (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
				float green = 0;// (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
				float blue = 0;// (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

				int index = 0;
				bool found = false;

				m_particleList[m_currentParticleCount].position = maths::vec3(positionX, positionY, positionZ);
				m_particleList[m_currentParticleCount].color = maths::vec4(red, green, blue, 1.0f);
				std::cout << m_particleList[m_currentParticleCount].position << std::endl;
				std::cout << m_particleList[m_currentParticleCount].color << std::endl;
				m_particleList[m_currentParticleCount].velocity = velocity;
				m_particleList[m_currentParticleCount].active = true;

				m_currentParticleCount++;



				return;
				while (!found)
				{
					if ((m_particleList[index].active == false) || m_particleList[index].position.z < positionZ)
						found = true;
					else
						index++;
				}

				if (index < m_maxParticleCount)
				{
					int i = m_currentParticleCount;
					int j = i - 1;

					std::cout << "index: " << index << std::endl;
					while (i != index)
					{
						m_particleList[i].position = m_particleList[j].position;
						m_particleList[i].color = m_particleList[j].color;
						m_particleList[i].velocity = m_particleList[j].velocity;
						m_particleList[i].active = m_particleList[j].active;

						i--;
						j--;
					}

					m_particleList[index].position = maths::vec3(positionX, positionY, positionZ);
					m_particleList[index].color = maths::vec4(red, green, blue, 1.0f);
					m_particleList[index].velocity = velocity;
					m_particleList[index].active = true;
				}
				
			}
		}

		void  ParticleSystem::UpdateBuffers()
		{
			memset(m_particleVertices, 0, sizeof(ParticleVertex) * m_vertexCount);

			int index = 0;

			for (int i = 0; i < m_currentParticleCount; ++i)
			{
				float positionX = m_particleList[i].position.x;
				float positionY = m_particleList[i].position.y;
				float positionZ = m_particleList[i].position.z;

				// 왼쪽 아래
				m_particleVertices[index].position = maths::vec3(positionX - m_size, positionY - m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(0, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 왼쪽 위
				m_particleVertices[index].position = maths::vec3(positionX - m_size, positionY + m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(0, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 오른쪽 아래
				m_particleVertices[index].position = maths::vec3(positionX + m_size, positionY - m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(1, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 오른쪽 아래
				m_particleVertices[index].position = maths::vec3(positionX + m_size, positionY - m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(1, 1);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;

				// 왼쪽 위
				m_particleVertices[index].position = maths::vec3(positionX - m_size, positionY + m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(0, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;
			
				// 오른쪽 위
				m_particleVertices[index].position = maths::vec3(positionX + m_size, positionY + m_size, positionZ);
				m_particleVertices[index].uv       = maths::vec2(1, 0);
				m_particleVertices[index].color    = m_particleList[i].color;
				index++;
			}

			m_vertexBuffer->SetData(m_vertexCount * sizeof(ParticleVertex), &m_particleVertices[0]);
		}
	}
}