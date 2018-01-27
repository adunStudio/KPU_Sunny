# VertexBuffer

수학적으로, 한 삼각형의 정점(vertex)은 두 변이 만나는 점이다. 선분의 경우 선분의 양 끝점이 정점이고, 하나의 점의 경우에는 그 점 자체가 정점이다.

정점은 그냥 기하학적 기본도형(primitive)의 꼭짓점이라는 인상을 받을 수 있다. 그러나 Direct3D의 정점은 그보다 훨씬 일반적이다. Direct3D의 정점은 본질적으로 공간적 위치 이외의 정보도 담을 수 있으며 이를 통해서 좀 더 복잡한 렌더링 효과를 낼 수 있다. 예를 들어 조명을 구현하기 위해 정점에 법선 벡터를 추가하며, 텍스처를 적용하기 위해 텍스처 좌표를 추가한다. Direct3D는 응용 프로그램이 자신만의 정점 형식을 정의할 수 있는 유연성을 제공한다.

정점들은 **정점 버퍼**라고 하는 특별한 Direct3D 자료구조 안에 담겨서 렌더링 파이프라인에 묶인다. 정점 버퍼는 그냥 일단의 정점들을 연속적인 메모리 저장하는 자료구조일 뿐이다. 정점 버퍼 자체는 그 정점들을 어떤 식으로 조합해서 기본도형을 형성할 것인지를 말해 주지 않는다. 예를 들어 한 정점 버퍼 안에 담긴 정점들을 두 개씩 엮어서 선분을 형성할 수도 있고, 세 개씩 엮어서 삼각형을 형성할 수도 있다. 기본도형을 형성하는 방식을 Direct3D에게 알려주는 데 쓰이는 수단이 바로 기본도형 위상구조(primitive topology)다.

---

Sunny Engine은 정점 버퍼 생성 과정을 추상화한 VertexBuffer 클래스가 존재합니다.

자세한 사용법은 cpp 파일에서 주석으로 대신합니다.

```cpp
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
```

---

### Commit
[Step 06. Buffers]()
