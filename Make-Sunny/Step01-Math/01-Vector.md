# Vector

고등 수학, 물리를 수료했다는 가정하에 자세한 개념과 공식은 이 설명에서 생략합니다.

- **스칼라**: 크기만을 나타내는 물리량
- **벡터**: 크기와 동시에 방향을 갖는 물리량

게임 개발에서는 벡터 연산(Vector Operation)을 자주 사용합니다. 벡터로서의 접근이 직관적이며, 방정식이나 복잡한 계산을 피하고 문제를 훨씬 쉽고 간단하며 효율적으로 해결 해주기 때문입니다. 그래서 게임 엔진을 만들 때 벡터연산의 적용을 가장 우선적으로 하는 것이 일반적입니다.

---
Sunney Engine은 아래와 같이 물리량의 벡터를 추상화한 구조체가 존재합니다.

```cpp
vec2(float x, float y);
tvec(T x, T y);
vec3(float x, float y, float z);
vec4(float x, float y, float z, float w);
```

---

기본적으로 모든 구조체는 아래 목록과 같은 벡터간의 기본적인 연산과 이에 맞는 연산자 오버라이딩이 구현되어있습니다.

- Add(operator +, +=)
- Subtract(operator -, -=)
- Multiply(operator *, *=)
- Divide(operator /, /=)
- operator ==
- operator !=
- operator >, >=, <, <=

```cpp
// Example : maths/vec3.h

vec3& Add(const vec3& other);
vec3& Subtract(const vec3& other);
vec3& Multiply(const vec3& other);
vec3& Divide(const vec3& other);

vec3& Add(float other);
vec3& Subtract(float other);
vec3& Multiply(float other);
vec3& Divide(float other);

friend vec3 operator+(vec3 left, const vec3& right);
friend vec3 operator-(vec3 left, const vec3& right);
friend vec3 operator*(vec3 left, const vec3& right);
friend vec3 operator/(vec3 left, const vec3& right);

friend vec3 operator+(vec3 left, float right);
friend vec3 operator-(vec3 left, float right);
friend vec3 operator*(vec3 left, float right);
friend vec3 operator/(vec3 left, float right);

bool operator==(const vec3& other) const;
bool operator!=(const vec3& other) const;

vec3& operator+=(const vec3& other);
vec3& operator-=(const vec3& other);
vec3& operator*=(const vec3& other);
vec3& operator/=(const vec3& other);

vec3& operator+=(float other);
vec3& operator-=(float other);
vec3& operator*=(float other);
vec3& operator/=(float other);

bool operator<(const vec3& other) const;
bool operator<=(const vec3& other) const;
bool operator>(const vec3& other) const;
bool operator>=(const vec3& other) const;

friend vec3 operator-(const vec3& vector);
```
---

또한 벡터 연산에서 가장 기본은 내적(Dot Product)과 외적(Cross Product)입니다.

[게임에서 내적과 외적(Dot Product and Cross Product in Game)](http://rapapa.net/?p=2974)

```cpp
// 내적 (내적의 결과값은 벡터가 아닌 스칼라)
float vec3::Dot(const vec3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

// 외적 (외적의 결과값은 두 벡터에 수직인 벡터)
vec3 vec3::Cross(const vec3& other) const
{
    return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}
```

---

이 외에도 다음과 같은 연산을 구현합니다.

- 벡터의 크기(Magnitude)
- 노말 벡터(Normalise)
- 벡터간의 거리(Distance)

```cpp
float vec3::Magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

vec3 vec3::Normalize() const
{
    float length = Magnitude();
    return vec3(x / length, y / length, z / length);
}

float vec3::Distance(const vec3& other) const
{
    float a = x - other.x;
    float b = y - other.y;
    float c = z - other.z;
    return sqrt(a * a + b * b + c * c);
}
```
---

### Commit
[Step 01. Maths - Vectors](https://github.com/adunStudio/Sunny/commit/564b17b4fecb0f3c1a431c4169518e528cde8d29)
