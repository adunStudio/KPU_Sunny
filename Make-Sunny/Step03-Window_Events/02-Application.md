# Application

Sunny Engine은 이전의 Window 클래스를 이용해서 게임 루프를 구현한 Application 클래스가 존재합니다.
- OnUpdateTick : 1초마다 호출됩니다.
- OnUpdate : 매 프레임마다 호출됩니다.
- OnRender : 가능한한 빨리 호출됩니다.

---

Window 클래스의 Run 멤버 함수를 Update로 수정 하며, 코드 또한 조금 바뀝니다.

```cpp
void Window::Update()
{
    MSG message;

    while(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE) > 0)
    {
        if (message.message == WM_QUIT)
        {
            m_closed = true;
            return;
        }

        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}
```

---

Run() 함수는 이제 Application 클래스에 존재합니다.

```cpp
void Application::Run()
{
    m_timer = new utils::Timer();

    float timer = 0.0f;
    float updateTimer = m_timer->ElapsedMillis();
    float updateTick = 1000.0f / 60.0f;

    int frames = 0;
    int updates = 0;

    utils::Timestep timestep(m_timer->ElapsedMillis());

    while(m_running)
    {
        window->Clear();

        float now = m_timer->ElapsedMillis();

        // updateTick(1000 / 60)마다 실행
        if(now - updateTimer > updateTick)
        {
            timestep.Update(now);
            OnUpdate(timestep);
            updates++;
            updateTimer += updateTick;
        }
        { // 가능한한 빠르게
            utils::Timer frametime;
            OnRender();
            frames++;
            m_frameTime = frametime.ElapsedMillis();
        }

        window->Update();

        // 1초마다 실행
        if(m_timer->Elapsed() - timer > 1.0f)
        {
            timer += 1.0f;
            m_framesPerSecond = frames;
            m_updatesPerSecond = updates;
            frames = 0;
            updates = 0;
            OnTick();
        }

        if(window->Closed())
            m_running = false;
    }
}
```

---

사용 방법은 아래와 같습니다.

```cpp
int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    sunny::app::Application game("Test", {800, 600, false, false});

    game.Start();

    return 0;
}

Hello, Sunny Project!
UPS: 60
FPS: 1278809
UPS: 60
FPS: 1229386
UPS: 60
FPS: 1182131
UPS: 60
FPS: 1226655
```

---

### Commit
[Step 03. Window & Events - Application](https://github.com/adunStudio/Sunny/commit/9a2898f95f2b4437b1a2ef92bfe6e06fd4016f68)
