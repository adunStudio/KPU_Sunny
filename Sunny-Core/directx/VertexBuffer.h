#pragma once

#include "../sunny.h"
#include "Common.h"
#include "Context.h"
#include "BufferLayout.h"
#include "Shader.h"

/*
GPU가 정점 배열에 접근할 수 있으려면 배열의 정점들을 버퍼(buffer)라고 부르는 특별한 자원에 담아 두어야 한다.
Direct3D 코드에서 버퍼는 ID3D11Buffer 인터페이스로 대표된다.

정점들을 담는 버퍼를 정점 버퍼라고 부른다. Direct3D의 버퍼들은 자료를 담을 뿐만아니라, CPU나 GPU가 자료에 어떻게 접근할 수 있고
버퍼가 파이프라인의 어디에 묶이는지에 대한 정보도 가진다. 정점 버퍼를 생성하려면 다음과 같은 과정을 거친다.

1. 생성할 버퍼를 서술하는 D3D11_BUFFER_DESC 구조체를 세운다.
2. 버퍼의 초기화에 사용할 자료를 서술하는 D3D11_SUBRESOURCE_DATA 구조체를 채운다.
3. ID3D11Device::CreateBuffer를 호출해서 버퍼를 생성한다.
*/


// 정점 버퍼를 사용하는 과정을 정리하면 다음과 같다.
// 1. 정점 버퍼를 생성한다.
// 2. 정점 버퍼를 입력 조립 단계에 연결한다.
// 3. 입력 레이아웃 객체를 생성한다.
// 4. 입력 레이아웃 객체를 입력 조립단계에 연결한다. 

/* 입력조립단계 (IA) */
/*
렌더링 파이프라인의 제일 처음 단계이다. 

어플리케이션으로부터 넘겨받은 정점버퍼나 인덱스 버퍼의 데이터를 읽어 들여 [점], [선], [삼각형]등의 프리미티브를
조합하여 파이프라인의 다음 단계로 데이터를 흘려보내는 역할을 한다.

그 밖에 셰이더에서의 처리에 필요한 [시스템 생성값]을 추가할 수 있다.
[시스템 생성값]은 다른 입출력 요소와 마찬가지로 [시멘틱스]를 붙여 식별하며
IA가 생성하느 시스템 생성값에는 [정점 ID] [프리미티브 ID] [인스턴스 ID]등이 있다.

IA에서 다루는 입력 버퍼에는 [정점 버퍼] 와 [인덱스 버퍼] 2 종류가 있으며 이 둘은 버퍼 리소스의 한 종류이다.
*/

namespace sunny
{
	namespace directx
	{
		enum class BufferUsage
		{
			// D3D11_USAGE 열거형 상수
			DEFAULT, DYNAMIC
			// DEFAULT : D3D11_USAGE_DEFAULT (GPU가 리소스를 읽고 쓸 수 있음을 나타낸다. 가장 일반적으로 사용하는 상수다.)
			// DYNAMIC : D3D11_USAGE_DYNAMIC (CPU는 쓰기만 할 수 있고 GPU는 읽기만 할 수 있는 리소스를 나타낸다.)
			//       ㄴ: CPU가 자주 리소스의 내용을 변경해야 하는 리소스를생성할 때 사용한다. 이러한 리소스를 동적 리소스라고 한다.
			//       ㄴ: 응용 프로그램이 동적 리소스의 내용을 변경하기 위하여 ID3D11DeviceContext::Map() 함수를 사용한다.
		};

		class VertexBuffer
		{
		private:
			D3D11_BUFFER_DESC m_bufferDesc;                  // 버퍼를 설정하는 구조체
			
			D3D11_MAPPED_SUBRESOURCE m_mappedSubresource;    // 서브 리소스 데이터에 접근하기 위한 구조체
			
			ID3D11Buffer* m_bufferHandle;                    // 정점 데이터를 저장하기 위한 정점 버퍼 인터페인스 포인터
			
			ID3D11InputLayout* m_inputLayout;                // 입력 레이아웃 인터페이스 포인터

			BufferUsage m_usage;                
			
			unsigned int m_size;                             // 정점 크기

			BufferLayout m_layout;

		protected:
			void* GetPointerInternal();

		public:
			VertexBuffer(BufferUsage usage = BufferUsage::DEFAULT);
			~VertexBuffer();

			void Resize(unsigned int size);
			
			void SetLayout(const BufferLayout& layout);
			
			void SetData(unsigned int size, const void* data);

			void ReleasePointer();

			void Bind();

			template<typename T>
			T* GetPointer()
			{
				return (T*)GetPointerInternal();
			}
		};
	}
}