#pragma once
#include "../../include.h"
#include "../../maths/maths.h"
#include "../../directx/Texture2D.h";
#include "../../directx/VertexBuffer.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/VertexArray.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		struct Particle
		{
			maths::vec3 position;
			maths::vec4 color;
			float velocity;
			bool active;
		};

		struct ParticleVertex
		{
			maths::vec3 position;          // 위치
			maths::vec4 color;             // 색깔
			maths::vec2 uv;                // 텍스처 위치
		};


		class ParticleSystem
		{
		protected:
			directx::Texture* m_texture;                      // 텍스처 

			directx::VertexArray*  m_vertexArray;
			directx::VertexBuffer* m_vertexBuffer;
			directx::IndexBuffer*  m_indexBuffer;

			Particle* m_particleList;
			ParticleVertex* m_particleVertices;

			unsigned int m_vertexCount;
			unsigned int m_indexCount;

		protected:
			int m_currentParticleCount;

			unsigned int m_maxParticleCount;

			maths::vec3 m_position;                           // 위치
			maths::vec3 m_rotate;                             // 회전
			maths::vec3 m_speed;                              // 속도
			maths::vec3 m_accelaration;                       // 가속도
			maths::vec4 m_color;                              // 컬러
			
			float m_size = 20.0f;                             // 크기 
			float m_fade;                                     // 퇴색 정도
			float m_age;                                      // 나이
			
			float m_accumulatedTime = 0;                      // 파티클이 방출되는 속도 누적 시간
			float m_particlePerSecond = 650.0f;               // 초당 방출 할 파티클 수
			float m_lifeTime;                                 // 파티클이 소멸될때까지 유지되는 시간

			bool m_visible;                                
		
		protected:
			void KillParticles();
			void EmitParticles(const utils::Timestep& ts);
			void UpdateBuffers();

		public:
			ParticleSystem(directx::Texture* texture);
			~ParticleSystem();
			
			virtual void Update(const utils::Timestep& ts);
			virtual void Render();

		public:
			inline const maths::vec3& GetPosition() const { return m_position; }
			inline void SetPosition(const maths::vec3& position) { m_position = position; }

			inline const maths::vec3& GetSpeed() const { return m_speed; }
			inline void SetSpeed(const maths::vec3& speed) { m_speed = speed; }

			inline const maths::vec3& GetAccelaration() const { return m_accelaration; }
			inline void SetAccelaration(const maths::vec3& accelaration) { m_accelaration = accelaration; }

			inline const maths::vec4& GetColor() const { return m_color; };
			inline void SetColor(const maths::vec4& color) { m_color = color; };

			inline const float GetSize() const { return m_size; }
			inline void SetSize(const float size){ m_size = size; }

			inline const float GetFade() const { return m_fade; }
			inline void SetFade(const float fade) { m_fade = fade; }

			inline const float GetLifeTime() const { return m_lifeTime; }
			inline void SetLifeTime(const float lifeTime) { m_lifeTime = lifeTime; }

			inline const float GetAge() const { return m_age; }
			inline void SetAge(const float age) { m_age = age; }

			inline const bool GetVisible() const { return m_visible; };
			inline void SetVisible(bool visible) { m_visible = visible; };
		};
	}

	
}