#pragma once

#include "../sunny.h"
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

		public:
			IndexBuffer(unsigned int* data, unsigned int count);

			void Bind() const;

			inline unsigned int GetCount() const { return m_count; };
		};
	}
}