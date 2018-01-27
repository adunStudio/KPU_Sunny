# VertexArray

앞서 만든 VertexBuffer, IndexBuffer 클래스는 버퍼의 생성과 입력 조립단계에 연결하는 기능을 수행합니다.

버퍼를 메모리의 한 입력 슬롯에 지정한다고 해서 저절로 그리기 작업이 진행되는 것은 아닙니다. 정점들의 그리기 연산을 실제로 시작하려면
- void ID3D11DeviceContext::Draw(UINT VertexCount, UINT StartVertexLocation);   (정점버퍼)
- void ID3D11DeviceContext::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);   (인덱스버퍼)

와 같은 메서드를 호출해야 합니다.

---

Sunny Engine은 VertexArray 클래스가 존재합니다. 이 클래스는 정점 버퍼들을 관리하며, 순회하며 그리기 연산을 수행하는 Draw 메서드를 가지고 있습니다.

```cpp
class VertexArray
{
private:
	std::vector<VertexBuffer*> m_buffers;

public:
	VertexArray();
	~VertexArray();

	inline VertexBuffer* GetBuffer(unsigned int index = 0) { return m_buffers[index]; }

	void PushBuffer(VertexBuffer* buffer);

	void Draw(unsigned int count) const;
};
```

```cpp
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
```

### Commit
[Step 06. Buffers]()
