#include "VertexBuffer.h"

namespace sunny
{
	namespace directx
	{
		VertexBuffer::VertexBuffer() : m_size(0)
		{
			ZeroMemory(&m_bufferDesc, sizeof(D3D11_BUFFER_DESC));

			                                                                                                               // ByteWidth : 생성할 정점 버퍼의 크기(바이트 단위)
			m_bufferDesc.Usage          = D3D11_USAGE_DYNAMIC;															   // Usage     : 버퍼가 쓰이는 방식 (D3D11_USAGE_DEFAULT  vs ..)
			m_bufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;                                                        // BindFlags : 정점 버퍼의 경우에는 D3D11_BIND_VERTEX_BUFFER를 지정한다.
			m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                                                          // CPUAccessFlags : CPU가 버퍼에 접근하는 방식을 결정하는 플래그 (CPU가 자료를 기록해서 버퍼를 갱신해야 한다면 D3D11_CPU_ACCESS_WRITE)
			                                                                                                               // MiscFlags : 정점 버퍼에 대해서는 이 기타 플래그들을 사용할 필요가 없다. (그냥 0)
																														   // StructureByteStride: 구조적 버퍼(structured buffer)에 젖아된 원소 하나의 크기(바이트 단위), 구조적 버퍼가 아닐경우는 0
		}  

		VertexBuffer::~VertexBuffer()
		{
		}

		void VertexBuffer::Resize(unsigned int size)
		{
			m_size = size;
			m_bufferDesc.ByteWidth = size;                                                                                 // ByteWidth : 생성할 정점 버퍼의 크기(바이트 단위)

			// 버퍼를 생성하기 위해서 ID3D11Device::CreateBuffer() 함수를 사용한다.
			Context::GetContext()->GetDevice()->CreateBuffer(&m_bufferDesc, NULL, &m_bufferHandle);
			// pDesc        : 생성할 버퍼를 설정하는 D3D11_BUFFER_DESC 구조체의 포인터
			// pInitialData : 생성할 버퍼를 초기화 하기 위한 데이터(D3D11_SUBRESOURCE_DATA)의 포인터, 이 값이 NULL이면 메모리만 할당하고 초기화하지 않는다. 
			// ppBuffer     : 생성된 버퍼 인터페이스를 반환할 포인터의 주소
		}

		void VertexBuffer::SetData(unsigned int size, const void* data)
		{
			if (m_size < size) Resize(size);

			GetPointerInternal();
			memcpy(m_mappedSubresource.pData, data, size);
			ReleasePointer();
		}

		void VertexBuffer::SetLayout(const BufferLayout& bufferLayout)
		{
			m_layout = bufferLayout;
			const std::vector<BufferElement>& layout = m_layout.GetLayout();

			// 정점 구조체의 각 성분(필드)이 어떤 용도인지를 Direct3D에게 알려 주어야 한다. 
			// 그러한 정보를 Direct3D에게 알려 주는 수단이 바로 ID3D11InputLayout 형식의 입력 배치(input layout) 객체다. 
			// 입력 배치는 D3D11_INPUT_ELEMENT_DESC 구조체들로 이루어진 배열을 통해서 구축한다.
			D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[layout.size()];

			for (unsigned int i = 0; i < layout.size(); i++)
			{
				const BufferElement& element = layout[i];
				desc[i] = { element.name.c_str(), 0, (DXGI_FORMAT)element.type, 0, element.offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				// SemanticName         : 성분에 부여된 문자열 이름. 이것은 정점 셰이더에서 sematic 이름으로 쓰이므로 반드시 유효한 변수 이름이어야 한다.
				// SematicIndex         : semantic에 부여된 인덱스. (하나의 정점 구조체에 텍스터 좌표가 여러 개 있는 경우 각 텍스처 좌표마다 개별적인 semantic을 부여하는 대신 이처럼 인덱스를 통해거 구별할 수 있다.) 셰이더 코드에서 인덱스가 지정되지 않은 semantic은 0으로 간주된다.
				// Fortmat              : DXGI_FORMAT 열거형의 한 멤버로, 이 정점 성분의 자료 형식을 나타낸다.
				// InputSlot            : 자료가 공급될 정점 버퍼 슬롯의 인덱스다. Direct3D는 16개의 정점 버퍼 슬롯들을 지원한다.
				// AligendByteOffset    : 입력 슬롯을 하나만 사용하는 경우 이 필드는 C++ 정점 구조체의 시작 위치와 이 정점 성분의 위치 사이의 오프셋(바이트 단위)이다.
				// InputSlotClass       : 보통 D3D11_INPUT_PER_VERTEX_DATA 값을 가진다.
				// InstanceDataStepRate : 보통 0의 값을 가진다.
			}

			const Shader* shader = Shader::CurrentlyBound();

			// 입력 배치 서술 구조체 배열을 완성하고 입력 배치를 위한 ID3D11InputLayout 인터페이스를 얻었다면, 
			// 이제 ID3D11Device::CreateInputLayout 메서드를 호출해서 입력 배치를 생성한다.
			Context::GetDevice()->CreateInputLayout(desc, layout.size(), shader->GetData().vs->GetBufferPointer(), shader->GetData().vs->GetBufferSize(), &m_inputLayout);
			// pInputElementDescs                : 정점 구조체를 서술하는 D3D11_INPUT_ELEMENT_DESC 들의 배열.
			// NumElements                       : 그 D3D11_INPUT_ELEMENT_DESC 배열의 원소 개수.
			// pShaderBytecodeWithInputSignature : 정점 셰이더(입력 서명을 포함한)를 컴파일해서 얻은 바이트 코드를 가리키는 포인터.
			// BytecodeLength                    : 그 바이트 코드의 크기(바이트 단위).
			// ppInputLayout                     : 생성된 입력 배치를 이 포인터를 통해서 돌려준다.
			delete desc;
		}

		void* VertexBuffer::GetPointerInternal()
		{
			// 서브 리소스에 포함된 데이터에 대한 포인터를 얻어오기위해 ID3D11DeviceContext::Map() 함수를 사용한다. 이 함수를 호출하면 GPU는 서브 리소스에 접근할 수 없게 된다.
			// 즉, 정점 버퍼에 대한 메모리 주소를 가져온다.
			Context::GetDeviceContext()->Map(m_bufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_mappedSubresource);
			// pResource       : ID3D11Resource 인터페이스에 대한 포인터다.
			// SubResource     : 서브 리소스의 인덱스
			// MapType         : 리소스에 대한 CPU의 읽기/쓰기 권한을 나타내는 D3D11_MAP 열겨형 상수다.        
			//    ㄴ             D3D11_MAP_WRITE_DISCARD → 리소스에 쓰려고 맵핑한다. 리소스의 이전 내용은 보존되지 않는다.
			// MapFlags        : GPU가 리로스에 접근하고 있을 때 CPU가 어떻게 할 것인가를 나타내는 상수다.
			// pMappedResource : 맵핑된 서브 리소스에 대한 포인터다.

			return m_mappedSubresource.pData;
		}

		void VertexBuffer::ReleasePointer()
		{
			Context::GetDeviceContext()->Unmap(m_bufferHandle, NULL);
		}

		void VertexBuffer::Bind()
		{
			// 정점 버퍼를 사용하려면 입력 레이아웃 인터페이스 객체를 생성하여 정점 버퍼의 레이아웃을 정의해야 한다.
			unsigned int offset = 0;
			unsigned int stride = m_layout.GetStride();
			
			// 이렇게 생성한 입력 배치는 아직 장치에 묶이지 않은 상태이다.
			// 따라서 입력 배치 설정의 마지막 단계는 입력 배치를 사용하고자 하는 장치에 묶는 것이다.
			Context::GetDeviceContext()->IASetInputLayout(m_inputLayout);

			// ID3D11DeviceContext::IASetVertexBuffers() 함수를 이용해 정점 버퍼를 입력 조립단계에 연결 한다.
			Context::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_bufferHandle, &stride, &offset);
			// StartSlot       : 연결할 첫 번째 입력 슬롯 인덱스다. 첫 번째 정점 버퍼는 시작 슬롯에 연결되고 순서대로 다음 슬롯에 연결된다. 최대 16개의 입력 슬롯을 제공한다.
			// NumBuffers      : ppVertexBuffers가 나타내는 배열의 원소 개수다. 버퍼의 개수는 입력 슬롯 전체 개수를 넘을 수 없다.
			// ppVertexBuffers : 정점 버퍼(ID3D11Buffer) 배열에 대한 포인터다. 정점 버퍼는 D3D11_BIND_VERTEX_BUFFER 플래그로 생성되어야 한다.
			// pStrides        : 정점 버퍼 배열의 원소의 크기(바이트)들의 배열에 대한 포인터다. 이 배열은 정점 버퍼의 개수(NumBuffers)만큼의 원소를 갖는다.
			// pOffsets        : 오프셋 값들의 배열에 대한 포인터다. 오프셋은 정점 버퍼에서 실제로 사용되는 첫번째 원소와 버퍼의 첫 번째 원소사이의 바이트 수다. 이 배열은 정점 버퍼의 개수(NumBuffers)만큼의 원소를 갖는다.
		}

	}
}