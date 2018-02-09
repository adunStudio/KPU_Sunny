#include "BufferLayout.h"

namespace sunny
{
	namespace directx
	{
		BufferLayout::BufferLayout() : m_size(0)
		{
		}

		void BufferLayout::Push(const std::string& name, unsigned int type, unsigned int size, unsigned int count)
		{
			m_layout.push_back({ name, type, size, count, m_size });
			m_size += size * count;
		}
	}
}