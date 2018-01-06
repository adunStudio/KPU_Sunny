# Vector

- *스칼라*: 스칼라는 크기만을 나타내는 물리량으로 길이, 넓이, 시간, 온도, 질량, 무게, 속력, 에너지 등을 말한다.
- *벡터*: 벡터는 크기와 동시에 방향을 갖는 물리량으로서 변위, 속도, 가속도, 힘, 운동량, 전기장, 자기장, 각운동량등을 말한다.

Sunney Engine은 아래와 같은 벡터 클래스를 지원합니다.

- maths/vec2
- maths/tvec2
- maths/vec3
- maths/vec4

이는 크기와 동시에 방향을 갖는 물리량의 벡터를 추상화한 클래스며, STL Container의 vector와는 의미가 다릅니다.


## 멤버 변수

### vec2
- float x, y

### tvec2 (template vec2)
- T x, y

### vec3
- float x, y, z

### vec4
- float x, y, z, w


## 멤버 함수

기본적으로 모든 클래스는 아래 목록과 같은 벡터간의 기본적인 연산을 지원하며, 연산자 오버라이딩도 되어있습니다.
- Add(operator+, +=)
- Subtract(operator-, -=)
- Multiply(operator*, *=)
- Divide(operator/, /=)
- operator==
- operator!=
- operator>, >=, <, <=
