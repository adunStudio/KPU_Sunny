# Timer

게임 개발에는 FPS(Frame Per Second) 확인, 특정 이벤트 수행 시간과 같이 이래저래 시간을 계측해야 하는 경우가 많습니다.

시간을 계층 하는 방법은 아래와 같이 다양한 방법이 있습니다.

- time.h: time()
- window.h: QueryPerformanceFrequenc(), timeGetTime()
- C++ STL chrono

차이는 있지만 많은 Performance Test에서 C++ STL chrono가 Release 모드에서 가장 우수한 성능을 나타내었습니다.

---

Sunny Engine은 C++ STL chrono를 사용한 Timer 클래스가 존재합니다.

```cpp
class Timer { };
```

> Timer 클래스를 분석하기 위해서는 chrono 라이브러이에 대한 선행지식이 필요합니다.

---

Timer 클래스는 다음과 같은 멤버 함수가 있습니다.

```cpp
public:
    // 현재 시간을 갱신한다.
    void Reset();

    // 경과된 시간을 second 단위로 반환한다. (소수점으로 표현)
    float Elapsed();

    // 경과된 시간을 milli second 단위로 반환한다.
    float ElapsedMillis();
```

간단한 사용 예시를 보여드리겠습니다.

```cpp
int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    sunny::utils::Timer timer;

    for(int i = 0; i < 1000000; ++i) { /* 필요한 작업 수행 */ }

    float elapsed_s  = timer.Elapsed();
    float elapsed_ms = timer.ElapsedMillis();

    std::cout << elapsed_s  << std::endl;
    std::cout << elapsed_ms << std::endl;

    return 0;
}

Hello, Sunny Project!
3.018
3018
```

---

### Commit
[Step 02. Timer & File - Timer](https://github.com/adunStudio/Sunny/commit/2625e48f792d47de2b6f93f59dedd1d898336d08)
