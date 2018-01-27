# BufferLayout

Direct3D의 정점은 공간적 위치 이외에 추가적인 자료를 가질 수 있다.

원하는 자료('특성')를 가진 커스텀 정점 형식을 만들려면 우선 그러한 자료를 담을 구조체를 정의해야 한다. 예를들어 다음은 서로 다른 두 종류의 정점 형식을 보여주는 예로, 하나는 위치와 색상으로 구성되고 다른 하나는 위치, 법선, 텍스처 좌표로 구성된다.

```cpp
struct Vertex1
{
  vec3 Pos;
  vec4 Color;
};

struct Vertex2
{
  vec3 Pos;
  vec3 Normal;
  vec2 tex0;
  vec2 tex1;
};
```

정점 구조체를 정의했다면, 다음으로는 그 정점 구조체의 각 성분(필드)이 어떤 용도인지를 Direct3D에게 알려 주어야 한다. 그러한 정보를 Direct3D에게 알려 주는 수단이 바로 ID3D11InputLayout 형식의 **입력 배치(input layout)** 객체다. 입력 배치는 D3D11_INPUT_ELEMENT_DESC 구조체들로 이루어진 배열을 통해서 구축한다. 이 D3D11_INPUT_ELEMENT_DESC 배열의 각 원소는 정점 구조체의 각 성분을 서술하는 역할을 한다.

INPUT_ELEMENT_DESC 구조체의 정의는 다음과 같다.
```cpp
typedef struct D3D11D3D11_INPUT_ELEMENT_DESC {
  LPCSTR SemanticName;
  UINT SemanticIndex;
  DXGI_FORMAT Format;
  UINT InputSlot;
  UINT AlignedByteOffset;
  D3D11_INPUT_CLASSIFICATION InputSlotClass;
  UINT InstanceDataStepRate;  
} D3D11_INPUT_ELEMENT_DESC;
```

- SemanticName: 성분에 부여된 문자열 이름. 이것은 정점 셰이더에서 sematic 이름으로 쓰이므로 반드시 유효한 변수 이름이어야 한다.
- SematicIndex: semantic에 부여된 인덱스. (하나의 정점 구조체에 텍스터 좌표가 여러 개 있는 경우 각 텍스처 좌표마다 개별적인 semantic을 부여하는 대신 이처럼 인덱스를 통해거 구별할 수 있다.) 셰이더 코드에서 인덱스가 지정되지 않은 semantic은 0으로 간주된다.
- Fortmat: DXGI_FORMAT 열거형의 한 멤버로, 이 정점 성분의 자료 형식을 나타낸다.
- InputSlot: 자료가 공급될 정점 버퍼 슬롯의 인덱스다. Direct3D는 16개의 정점 버퍼 슬롯들을 지원한다.
- AligendByteOffset: 입력 슬롯을 하나만 사용하는 경우 이 필드는 C++ 정점 구조체의 시작 위치와 이 정점 성분의 위치 사이의 오프셋(바이트 단위)이다.
```cpp
struct Vertex2
{
    vec3 Pos;      // 0바이트 오프셋
    vec3 Normal;   // 12바이트 오프셋(sizeof(float) * 3)
    vec2 tex0;     // 24바이트 오프셋
    vec2 tex1;     // 32바이트 오프셋(sizeof(float) * 2)
};
```
- InputSlotClass: 보통 D3D11_INPUT_PER_VERTEX_DATA 값을 가진다.
- InstanceDataStepRate: 보통 0의 값을 가진다.

---

입력 배치 서술 구조체 배열을 완성하고 입력 배치를 위한 ID3D11InputLayout 인터페이스를 얻었다면, 이제 ID3D11Device::CreateInputLayout 메서드를 호출해서 입력 배치를 생성한다.

- pInputElementDescs: 정점 구조체를 서술하는 D3D11_INPUT_ELEMENT_DESC 들의 배열.
- NumElements: 그 D3D11_INPUT_ELEMENT_DESC 배열의 원소 개수.
- pShaderBytecodeWithInputSignature: 정점 셰이더(입력 서명을 포함한)를 컴파일해서 얻은 바이트 코드를 가리키는 포인터.
- BytecodeLength: 그 바이트 코드의 크기(바이트 단위).
- ppInputLayout: 생성된 입력 배치를 이 포인터를 통해서 돌려준다.

---

Sunny Engine은 D3D11D3D11_INPUT_ELEMENT_DESC 와 ID3D11InputLayout 사용을 위한 구조체와 클래스가 존재합니다.

```cpp
struct BufferElement
{
  std::string name;
  unsigned int type;   // 자료 형식 (Format:DXGI_FORMAT)
  unsigned int size;   // 사이즈
  unsigned int count;  // 개수
  unsigned int offset; // 오프셋    (m_size += size * count) (AlignedByteOffset)
};

class BufferLayout { ... };
```

입력 배치를 설정하는 방법은 다음과 같습니다.

```cpp
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
	Context::GetDevice()->CreateInputLayout(desc, layout.size(), shader->GetData().vs->GetBufferPointer(), shader->GetData().vs->GetBufferSize(), &m_inputLayout));
	// pInputElementDescs                : 정점 구조체를 서술하는 D3D11_INPUT_ELEMENT_DESC 들의 배열.
	// NumElements                       : 그 D3D11_INPUT_ELEMENT_DESC 배열의 원소 개수.
	// pShaderBytecodeWithInputSignature : 정점 셰이더(입력 서명을 포함한)를 컴파일해서 얻은 바이트 코드를 가리키는 포인터.
	// BytecodeLength                    : 그 바이트 코드의 크기(바이트 단위).
	// ppInputLayout                     : 생성된 입력 배치를 이 포인터를 통해서 돌려준다.
	delete desc;
}
```

입력 배치를 생성합니다.

```cpp
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
```

---

### Commit
[Step 06. Buffers](https://github.com/adunStudio/KPU_Sunny/commit/2848ec848c487c0be9d535ecc81add5c8f61f9ba)
