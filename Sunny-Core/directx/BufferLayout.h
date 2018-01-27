#pragma once

#include "../sunny.h"
#include "../maths/maths.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		struct BufferElement
		{
			std::string name;
			unsigned int type;        // 자료 형식 (Format:DXGI_FORMAT)
			unsigned int size;        // 사이즈    
			unsigned int count;       // 개수
			unsigned int offset;      // 오프셋    (m_size += size * count) (AlignedByteOffset)
		};

		class BufferLayout
		{
		private:
			unsigned int m_size;
			std::vector<BufferElement> m_layout;
		
		public:
			BufferLayout();

			inline const std::vector<BufferElement>& GetLayout() const { return m_layout; }
			inline unsigned int GetStride() const { return m_size; }

			template<typename T>
			void Push(const std::string& name, unsigned int count = 1)
			{
				// TODO: Debug System
			}

			// float
			template<>
			void Push<float>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS, sizeof(float), count);
				// 
			}

			// unsigned int
			template<>
			void Push<unsigned int>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R32_UINT, sizeof(unsigned int), count);
			}

			// unsigned char
			template<>
			void Push<unsigned char>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(unsigned char), count);
			}

			// vec2
			template<>
			void Push<maths::vec2>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R32G32_FLOAT, sizeof(maths::vec2), count);
			}

			// vec3
			template<>
			void Push<maths::vec3>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R32G32B32_FLOAT, sizeof(maths::vec3), count);
			}

			// vec4
			template<>
			void Push<maths::vec4>(const std::string& name, unsigned int count)
			{
				Push(name, DXGI_FORMAT_R32G32B32A32_FLOAT, sizeof(maths::vec4), count);
			}

		private:
			void Push(const std::string& name, unsigned int type, unsigned int size, unsigned int count);
		};
	}
}