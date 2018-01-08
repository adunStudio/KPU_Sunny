# Framework VS Library

모든 게임에는 매 프레임마다 호출되는 게임 루프가 존재합니다. 여기서 대게 게임 루프는 직접 만들거나, 프레임워크를 사용합니다.

Library와 Framework의 차이는 제어를 누가하느냐에 따라 다릅니다. (제어의 역전)

- **Library**: 개발자가 직접 만든 게임 루프안에서 라이브러리 함수를 호출합니다. 즉, **게임 루프 제어를 개발자가 직접합니다.**
- **Framework**: 프레임워크 내부에서 스스로 게임 루프를 돌고 있으며, 개발자가 만든 함수(코드)를 호출합니다. 즉, **게임 루프 제어를 프레임 워크가 합니다.**

---

Sunny Engine과 상관없이 차이를 코드로 살펴보겠습니다.

#### 간단한 게임 루프

아래 코드는 순수하게 직접 만든 게임 루프를 사용합니다. 좀 더 작성해야 할 코드가 많지만, 개발자 의도대로 **완전한 제어**가 가능합니다.

```cpp
class Game
{
private:
    double getCurrentTime() { ... }; // 현재 시간을 반환

public:
    double preTime = getCurrentTime();
    double lag = 0.0;
    // ...code

    void run();
};

void Game::run()
{
    while(true) // 직접 만든 게임 루프
    {
        double cur = getCurrentTime();
        double elapsed = current - preTime;
        preTime = cur;
        lag += elapsed;

        while(lag >= FPS)
        {
            // ...
            lag -= MS_PER_UPDATE;
        }
    }
}
```

### Library

위키 문서에 따르면 라이브러리의 정의는 다음과 같습니다.

> 공통으로 사용할 수 있는 기능(들)을 모듈화 한 것이다.

cmath도 라이브러리로써 C++의 표준 라이브러리에 해당합니다. 라이브러리를 사용해보겠습니다.

```cpp
while(lag >= FPS)
{
    player.x = std::max(player.x - speed, 0);
    lag -= MS_PER_UPDATE;
}
```
cmath 라이브러리 안의 특정한 기능 max 함수를 호출했습니다. 이와 같이 게임 루프에서 라이브러리 함수를 호출 했지만 게임 루프에 대한 제어는 여전히 개발자에게 있습니다.

### Framework

대표적인 게임 프레임워크로는 cocos2d와 OpenGL의 GLUT 프레임워크가 있습니다.

```cpp
/** cocos2d Example **/

class GameWorld : public cocos2d:Layer
{
public:
    void update(float) override; // 오버라이딩
    // ... more
};

// 오버라이딩
void GameWorld::update(float delta)
{
    if(direction == LEFT)
    {
        player->x = std::max(palyer->x - speed, 0);
    }
}

/** GLUT Example **/

glutIdleFunc([]() {
    if(direction == LEFT)
    {
        player->x = std::max(palyer->x - speed, 0);
    }  
});
glutmainloop();
```

cocos2d Framework의 경우 update 멤버 함수는 게임 루프에 해당하며, 오버라이딩 할 경우 매 프레임마다 cocos2d Framework의 제어에 의해 실행됩니다. GLUT Framework의 경우 glutIdleFunc 함수는 아무것도 하지 않는 상태일 경우 자동으로 호출되는 것으로 유사 게임 루프에 해당합니다.

이처럼 자동으로 호출되는 함수코드(제어의 역전)는 까다로운 게임 루프 코드를 직접 작성하지 않아도 된다는 장점이 있지만 게임 루프 그 자체에 대한 제어가 불가능하므로 아쉬운게 있어도 건드릴 수 없다는 단점이 있습니다.

---

Sunny Engine은 게임 루프, 사용자 입력 이벤트, 렌더링에 대한 제어를 가지고 있으며 충돌 검사, UI, 게임 오브젝트등의 함수 및 클래스를 라이브러리 형태로 제공할 계획입니다.

---

### Commit
[Step 02. Window - Framework VS Library](https://github.com/adunStudio/Sunny/commit/29688a8fe66ddb2d1dbb1c2e879cce62f4f2b201)
