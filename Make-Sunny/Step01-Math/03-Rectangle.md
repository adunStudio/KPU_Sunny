# Rectangle

기본적인 사각형 구조체 입니다.

주로 2D에서 Button등의 UI를 표현하는데 사용하며, 마우스 포인트(vec2)와의 충돌처리를 수행합니다.

```cpp
struct Rectangle { vec2 position; vec2 size; };
```

---

공용체(union) 키워드를 사용해서 position(x, y), size(width, height)에 접근 가능합니다.

- Sunney Engine은 사각형의 위치(position) anchor point(or pivot)을 center로 잡습니다.
- 크기(size)는 half-width, half-height를 의미합니다.

```cpp
struct Rectangle
{
    union
    {
        vec2 position;
        struct {
            float x;
            float y;
        };
    };
    union
    {
        vec2 size;
        struct {
            float width;
            float height;
        };
    };
};
```

---

충돌판정은 다음과 같은 방식으로 수행합니다.

- Intersetcs (vs Rectangle) (AABB 방식)
- Contains (vs Vec2, Vec3)  (Bounding Box 방식)

```cpp
bool Rectangle::Intersects(const Rectangle& other) const
{
    return (size > other.position && position < other.size) || (position > other.size && size < other.position);
}

bool Rectangle::Contains(const vec2& point) const
{
    return point > GetMinimumBound() && point < GetMaximumBound();
}

bool Rectangle::Contains(const vec3& point) const
{
    return Contains(vec2(point));
}
```

---

### Commit
[Step 01. Maths - Rectangle](https://github.com/adunStudio/Sunny/commit/d66443b2c31cf981508041ab9fa2fb6177263e31)
