# AABB

AABB(Axis-Aligned Bounding Box)

일명 축 방향으로 정렬된 경계 상자 방식 충돌 체크는 충돌 체크 중 가장 기본이면서 간단하고 쉬운 방법입니다. 이 방식은 전통적으로 X 축에 대하여 겹치는지 체크하고 Y 축에 대하여 겹치는지 체크합니다. 3차원으로 갔을때는 축이 1개 더 (Z 축) 늘어나 축 하나당 조건이 2개씩 늘어나므로 3D 상에서는 총 6가지 조건을 체크하게 됩니다.

---

Sunny Engine에서는 vec2, vec3 구조체를 이용한 2D, 3D 상에서의 AABB 충돌 체크를 해주는 구조체가 존재합니다.

```cpp
struct AABB { vec3 min; vec3 max; };
```

---

2D, 3D 모두 지원 하므로 생성자는 오버로드 되어있으며, 사각형을 이용한 AABB 구조체 생성도 가능합니다.

```cpp
AABB();
AABB(const Rectangle& rectangle);
AABB(const vec2& min, const vec2& max);
AABB(const vec3& min, const vec3& max);
AABB(float x, float y, float width, float height);
AABB(float x, float y, float z, float width, float height, float depth);
```

---

충돌 체크는 기본적으로 Rectangle 구조체와 멤버 함수 이름이 같고 방식또한 같습니다.

```cpp
bool Intersects(const AABB& other) const;
bool Contains(const vec2& point) const;
bool Contains(const vec3& point) const;
```

---

[Step 01. Maths - AABB]()
