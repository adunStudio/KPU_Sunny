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

	

			ShaderFactory::Default3DForwardShader()->Bind();

			directx::VertexBuffer* buffer = new directx::VertexBuffer();
			buffer->SetData(m_vertexCount * sizeof(ParticleVertex), &m_particleVertices[0]);

			directx::BufferLayout layout;
			layout.Push<maths::vec3>("POSITION");
			layout.Push<maths::vec2>("TEXCOORD");
			layout.Push<maths::vec3>("COLOR");
			buffer->SetLayout(layout);

			directx::VertexArray* va = new directx::VertexArray();
			va->PushBuffer(buffer);

			directx::IndexBuffer* ib = new directx::IndexBuffer(&indices[0], m_indexCount);

		}

		ParticleSystem::~ParticleSystem()
		{

		}

		void ParticleSystem::Render()
		{

		}
	}
}