#include "IndexBuffer.h"

namespace sunny
{
	namespace directx
	{
		IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count) : m_count(count)
		{
			// 인덱스 버퍼를 서술하는 구조체를 채운다. (전체적으로 정점 버퍼와 비슷하다.)
			D3D11_BUFFER_DESC ibd;
			ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

			ibd.Usage = D3D11_USAGE_IMMUTABLE;                               // D3D11_USAGE_IMMUTABLE : 불변을 의미한다.
			ibd.ByteWidth = count * sizeof(unsigned int);                      
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;                         // 인덱스 버퍼의 경우에는 D3D11_BIND_INDEX_BUFFER를 지정한다.
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;

			// 인덱스 버퍼를 초기화할 자료를 지정한다.
			D3D11_SUBRESOURCE_DATA ibInitData;
			ibInitData.pSysMem = data;                                       // pSysMem: 버퍼를 초기화할 자료를 담은 시스템 메모리 배열을 가리키는 포인터


			HRESULT hr = Context::GetDevice()->CreateBuffer(&ibd, &ibInitData, &m_bufferHandle);
		}

		void IndexBuffer::Bind() const
		{
			// 입력 조립기 단계에 프리미티브 타입을 설정한다.
			Context::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : 위상 구조가 삼각형 목록임을 뜻한다.


			// 인덱스 버퍼를 사용하려면 먼저 인덱스 버퍼를 파이프 라인에 묶어야 한다. 인덱스 버퍼는 입력 조립기 단계에 묶어야 한다.
			Context::GetDeviceContext()->IASetIndexBuffer(m_bufferHandle, DXGI_FORMAT_R32_UINT, 0);
			// DXGI_FORMAT_R32_UINT: 32비트 부호 없는 정수를 뜻한다.
			// ㄴ D3D11_BUFFER_DESC::ByteWidth 멤버와 관련이 있으므로, 그 둘이 모순되지 않도록 하는 것이 중요하다.
		}

	}
}