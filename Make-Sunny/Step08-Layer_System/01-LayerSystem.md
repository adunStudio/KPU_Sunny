# Layer System

#### Layer는 한 화면위에 겹쳐져서 놓일 수 있는 투명한 판 정도로 이해하면 된다.

Sunny Engine은 Layer 클래스가 존재한다.
```cpp
class Layer
{
protected:
	Window* m_window;

	bool m_visible;

public:
	Layer();
	virtual ~Layer();

	inline bool IsVisible() const { return m_visible; }
	inline bool SetVisible(bool visible) { m_visible = visible; }

	virtual void Init();
	virtual void OnTick();
	virtual void OnUpdate(const utils::Timestep& ts);
	virtual void OnUpdateInternal(const utils::Timestep& ts);
	virtual void OnRender();
};
```

---

Application 클래스는 Layer들을 관리할 수 있는 동적 배열을 멤버로 가지고 있다.

```cpp
std::vector<Layer*> m_layerStack;
```

동적 배열에 대한 접근은 다음과 같은 메서드가 존재한다.

```cpp
void PushLayer(Layer* layer);
Layer* PopLayer();
Layer* PopLayer(Layer* layer);
```

이제 OnTick, OnUpdate, OnRender은 다음과 같이 바뀐다.

```cpp
void Application::OnTick()
{
	for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
		m_overlayStack[i]->OnTick();

	for (unsigned int i = 0; i < m_layerStack.size(); ++i)
		m_layerStack[i]->OnTick();
}

void Application::OnUpdate(const utils::Timestep& ts)
{
	for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
		m_overlayStack[i]->OnUpdateInternal(ts);

	for (unsigned int i = 0; i < m_layerStack.size(); ++i)
		m_layerStack[i]->OnUpdateInternal(ts);
}

void Application::OnRender()
{
	for (unsigned int i = 0; i < m_layerStack.size(); ++i)
		if (m_layerStack[i]->IsVisible())
			m_layerStack[i]->OnRender();

	for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
		if (m_overlayStack[i]->IsVisible())
			m_overlayStack[i]->OnRender();
}
```

---

사용자는 Layer클래스를 상속받아 하나이상의 레이어를 작성할 수 있다.

```cpp
class TestLayer : public Layer
{
public:
	TestLayer();
	~TestLayer();

	void Init() override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender() override;
};
```

---

main은 다음과 같이 바뀐다.

```cpp
class Game : public Application
{
public:
	Game() : Application("Test", { 800, 600, false, false })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		PushLayer(new TestLayer());
		PushLayer(new TestLayer());
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

    return 0;
}
```

---
### Commit

[Step 08. LayerSystem](https://github.com/adunStudio/KPU_Sunny/commit/022822e106e8159eb20bb0d6e3b890b36f3a83d1)
