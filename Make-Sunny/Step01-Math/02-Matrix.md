# Matrix

게임 수학 강의를 수강했다는 가정하에 자세한 개념과 공식은 이 설명에서 생략합니다.

- 3차원 물체를 나타낼때는 x, y, z 축과 관련된 회전, 위치 크기 등의 값이 존재한다.
- 월드 변환, 뷰 변환 투영 변환 등은 축소확대, 회전, 이동 등과 같은 기본적인 변환들을 조합하여 만들어진다.

위에서의 값들은 독립적인 값들이 아닌 서로가 서로에게 영향을 미칠 수 있는 값들입니다. 이러한 값들을 수학과 좀 더 연계한 동차좌표계(homogeneous coordinate)를 이용하여 이동, 축소확대, 회전 변환 모두 4 × 4 행렬 하나로 표현이 가능합니다.

- 크기 변환 행렬
- 회전 변환 행렬
- 이동 변환 행렬

---

Sunney Engine은 동차좌표계의 행렬을 추상화한 구조체가 존재합니다.

```cpp
struct mat4 { float elements[4 * 4]; };
```

---

오버로딩된 3가지 생성자와 함께 단위 행렬을 반환하는 Identity() 스태틱 함수를 가지고 있습니다.

```cpp
mat4();
mat4(float diagonal);
mat4(float* elements);
static mat4 Identity();

// mat4.cpp

mat4::mat4(float diagonal)
{
    memset(elements, 0, 4 * 4 * sizeof(float));
    elements[0 + 0 * 4] = diagonal;
    elements[1 + 1 * 4] = diagonal;
    elements[2 + 2 * 4] = diagonal;
    elements[3 + 3 * 4] = diagonal;
}

// 단위 행렬을 반환한다.
mat4 mat4::Identity()
{
    return mat4(1.0f);
}

```
|Identity|1 col|2 col|3 col|4 col|
|:---:|:---:|:---:|:---:|:---:|
| 1 row | 0 | 0 | 0 | 0 |
| 2 row | d | 0 | 0 | 0 |
| 3 row | 0 | d | 0 | 0 |
| 4 row | 0 | 0 | d | 0 |


---

mat4 구조체는 4 x 4 행렬을 추상화 하였으므로 총 16개의 원소를 가지고있습니다. 원소 데이터를 float[16] : 1차원 배열로 할당하고, C++ 공용체(union) 키워드를 사용하여 행(row)에 접근합니다.

| 1 row |     |     |     | 2 row |     |     |     | 3 row |     |      |      | 4row |     |      |      |
|:-----:|:---:|:---:|:---:|:-----:|:---:|:---:|:---:|:-----:|:---:|:----:|:----:|:----:|:---:|:----:|:----:|
|  [0]  | [1] | [2] | [3] |  [4]  | [5] | [6] | [7] |  [8]  | [9] | [10] | [11] | [12] | [13 | [14] | [15] |


```cpp
struct mat4
{
  union
  {
    // [row + col * 4]
    float elements[4 * 4];
    vec4 rows[4];
  };

  // more...
};
```



```cpp
vec4 mat4::GetColumn(int index) const
{
    return vec4(elements[index + 0 * 4], elements[index + 1 * 4], elements[index + 2 * 4], elements[index + 3 * 4]);
}

void mat4::SetColumn(uint index, const vec4& column)
{
    elements[index + 0 * 4] = column.x;
    elements[index + 1 * 4] = column.y;
    elements[index + 2 * 4] = column.z;
    elements[index + 3 * 4] = column.w;
}
```

---
