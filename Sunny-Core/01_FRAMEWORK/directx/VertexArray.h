#pragma once

#include "../include.h"
#include "VertexBuffer.h"

namespace sunny
{
	namespace directx
	{
		class VertexArray
		{
		private:
			std::vector<VertexBuffer*> m_buffers;

			DIMENSION m_dimension;

		public:
			VertexArray(DIMENSION dimension = DIMENSION::D3);
			~VertexArray();

			inline VertexBuffer* GetBuffer(unsigned int index = 0) { return m_buffers[index]; }

			void PushBuffer(VertexBuffer* buffer);

			void Draw(unsigned int count) const;
			void DrawOne(unsigned int count, unsigned char frame) const;
		};
	}
}