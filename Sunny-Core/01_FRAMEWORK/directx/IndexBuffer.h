#pragma once

#include "../include.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class IndexBuffer
		{
		private:
			ID3D11Buffer* m_bufferHandle;

			unsigned int m_count;

			DIMENSION m_dimension;

		public:
			IndexBuffer(unsigned int* data, unsigned int count, DIMENSION dimension = DIMENSION::D3);

			void Bind() const;

			inline unsigned int GetCount() const { return m_count; };
		};
	}
}