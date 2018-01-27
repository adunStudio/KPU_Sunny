#include "VertexArray.h"

namespace sunny
{
	namespace directx
	{
		VertexArray::VertexArray()
		{}

		VertexArray::~VertexArray()
		{}

		void VertexArray::PushBuffer(VertexBuffer* buffer)
		{
			m_buffers.push_back(buffer);
		}

		void VertexArray::Draw(unsigned int count) const
		{
			for (unsigned int i = 0; i < m_buffers.size(); ++i)
			{
				VertexBuffer* buffer = static_cast<VertexBuffer*>(m_buffers[i]);

				buffer->Bind();

				// 정점들의 그리기 연산을 실제로 시작하려면 다음 메서드를 호출히야 한다.
				// void ID3D11DeviceContext::Draw(UINT VertexCount, UINT StartVertexLocation);
				// VertexCount         : 정점 개수
				// StartVertexLocation : 시작 위치


				// 인덱스 버퍼를 이용해서 기본 도형을 그릴 때에는 Draw 메서드 대신 DrawIndexed 메서드를 사용해야 한다.
				// void ID3D11DeviceContext::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
				// IndexCount         : 이 그리기 호출에서 사용할 인덱스의 개수이다. 이것이 인덱스 버퍼의 전체 크기(모든 인덱스의 개수)일 필요는 없다. 즉, 인덱스들의 연속된 한 부분집한만 사용하는 것이 가능하다.
				// StartIndexLocation : 사용할 첫 인덱스의 위치(버퍼 안에서의 인덱스). 
				// BaseVertexLocation : 정점들을 가져오기 전에, 이 그리기 호출에서 사용할 인덱스들에 더해지는 정수 값.
				Context::GetDeviceContext()->DrawIndexed(count, 0, 0);
			}
		}

	}
}
