# Introduction to Shader

> 셰이더란 화면에 출력할 픽셀의 위치와 색상을 계산하는 함수

셰이더(shader) : '색의 농담, 명암 효과를 주다.'라는 뜻의 shade란 동사와 행동의 주체를 나타내는 접미사 '-er'을 혼합해 만든 단어

- 정점셰이더의 주된 임무는 각 정점의 공간을 변환 하는 것
- 픽셀셰이더의 주된 임무는 화면에 출력할 최종 색상을 계산하는 것

셰이더 프로그래밍이란 정점셰이더와 픽셀셰이더에 사용할 함수를 만는 것


셰이더는 HLSL(High Level Shading Language, 고수준 셰이딩 언어)이라고 하는 언어로 작성한다. 이 언어는 C++과 비슷하기 때문에 C++ 프로그래머라면 쉽게 배울 수 있다.

---

### 정점 셰이더

```cpp
cbuffer cbPerObject
{
  float4x4 gWorldViewProj;
};

void VSMain(float3 iPosL : POSITION, float 4 iColor : COLOR, out float4 oPosH : SV_POSITION, out float4 oClolor : COLOR)
{
  // 동차 절단 공간으로 변환한다.
  oPosH = mul(float4(iPosL, 1.0f), gWorldViewProj);

  // 정점 색상을 그대로 픽셀 셰이더에 전달한다.
  oColor = iColor;
}
```

하나의 정점 셰이더는 본질적으로 하나의 함수이다. 위의 hlsl 코드에서 VSMain라는 이름의 함수가 바로 정점 셰이더인데, 반드시 이 이름을 사용해야 하는 것은 아니다.

위의 정점 셰이더는 매개변수가 네 개인데, 처음 둘은 **입력** 매개변수이고 나머지 둘은 **출력** 매개변수(out 키워드)이다. HLSL에는 참조나 포인터가 없으므로, 함수가 여러 개의 값들을 돌려주려면 구조체를 사용하거나 이처럼 out이 지정된 매개변수를 사용해야 한다.

정점 셰이더의 처음 두 입력 매개변수는 정점 셰이더의 입력 서명을 형성하며, 커스텀 정점 구조체의 자료 멤버들에 대응된다. 매개변수 의미소 : POSITION과 : COLOR는 정점 구조체의 멤버들을 정점 셰이더 입력 매개변수들에 대응시키는 역할을 한다.

```cpp
struct Vertex
{
  vec3 Pos;
  vec4 Color;
};

D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
{
  {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
  {"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
}
```

출력 매개변수에도 의미소가 부여되어 있다(: SV_POSITION과 : COLOR). 이들은 정점 셰이더의 출력을 파이프라인의 다음 단계(기하 셰이더 또는 픽셀 셰이더)의 해당 입력에 대응시키는 역할을 한다. SV_POSITION 의미소는 특별한 의미소임을 주목하자. SV는 이것이 system value(시스템 값) 의미소임을 뜻한다. 이 의미소는 정점 셰이더의 이 출력이 정점 위치를 담고 있음을 말해 준다. 정점 위치는 정점의 다른 특성들은 관여하지 않는 연산들에 쓰이기 때문에 다른 특성들과는 다른 방식으로 처리해야 한다.

다음은 이 정점 셰이더와 정확히 같은 일을 하는 정점 셰이더로, 다른 점은 반환 형식과 입력 서명에 구조체들을 사용한다는 것이다(긴 매개변수 목록을 사용하는 대신).

```cpp
cbuffer cbPerObject
{
  float4x4 gWorldViewProj;
};

struct VertexIn
{
  float3 PosL  : POSITION;
  float4 COLOR : COLOR;
};

struct VertexOut
{
  float3 PosH  : SV_POSITION;
  float4 COLOR : COLOR;
};

VertexOut VSMain(VertexIn vin)
{
  VertexOut vout;

  vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

  // 정점 색상을 그대로 픽셀 셰이더에 전달한다
  vout.Color = vin.Color;

  return vout;
}
```

---

### 상수 버퍼

위 정점 셰이더에는 다음과 같은 코드가 있었다.

```cpp
cbuffer cbPerObject
{
  float4x4 gWorldViewProj;
};
```

위 코드는 cbuffer(constant buffer의 약자) 형식의 객체 cbPerObject를 정의한다. 상수 버퍼(constant buffer)는 셰이더가 접근할 수 있는 다양한 자료를 저장하는 유연한 자료 블록이다.


---

### 픽셀 셰이더

정점 셰이더처럼 픽셀 셰이더도 본질적으로 하나의 함수이다. 중요한 차이는 각 정점이 아니라 각 픽셀 단편마다 실행된다는 것이다. 픽셀 셰이더의 주된 임무는 주어진 입력으로부터 픽셀 단편의 생삭을 계산해내는 것이다.

```cpp
cbuffer cbPerObject
{
  float4x4 gWorldViewProj;
};

struct VertexIn
{
  float3 PosL  : POSITION;
  float4 COLOR : COLOR;
};

struct VertexOut
{
  float3 PosH  : SV_POSITION;
  float4 COLOR : COLOR;
};

VertexOut VSMain(VertexIn vin)
{
  VertexOut vout;

  vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

  // 정점 색상을 그대로 픽셀 셰이더에 전달한다.
  vout.Color = vin.Color;

  return vout;
}

float4 PSMain(VertexOut pin) : SV_Target
{
  return pin.Color;
}
```

위 코드에서 픽셀 셰이더 함수는 그냥 색상 값을 돌려준다. 픽셀 셰이더의 입력이 정점 셰이더의 출력과 정확히 일치해야 한다. 픽셀 셰이더는 하나의 4차원 색상 값을 돌려주며, 함수의 매개변수 목록 다음에 지정된 SV_TARGET이라는 의미소는 이 함수의 반환값 형식이 렌더 대상 형식과 일치해야 함을 뜻한다.

---

## 셰이더 컴파일하기

셰이더 프로그램을 실제로 사용하려면, 셰이더 프로그램들을 컴파일해야 한다. 이를 위해 다음과 같은 D3DX 함수를 사용한다.

```cpp
HRESULT D3DX11CompileFromFile(
  LPCTSTR pSrcFile,
  CONST D3D10_SHADER_MACRO* pDefines,
  LPD3D10INCLUDE pInclude,
  LPCSTR pFunctionName,
  LPCSTR pProfile,
  UINT Flags1,
  UINT Flags2,
  ID3DX11ThreadPump* pPump,
  ID3D10Blob** ppShader,
  ID3D10Blob** ppErrorMsgs,
  HRESULT* pHResult
);
```

- pFileName: 컴파일 셰이더 소스 코드를 담고 있는 파일의 이름
- pDefines: 사용하지 않는 고급 옵션이다. 항상 NULL 값을 지정한다.
- pInclude: 사용하지 않는 고급 옵션이다. 항상 NULL 값을 지정한다.
- pFunctionName: 셰이더 프로그램의 진입점
- pProfile: 사용할 셰이더 버전을 뜻하는 문자열
- Flags1: 셰이더 코드의 컴파일 방식에 영향을 미치는 플래그를 지정
- Flags2: 사용하지 않는 고급 옵션이다.
- pPump: 사용하지 않는 고급 옵션이다.
- ppShader: 컴파일된 셰이더를 담을 ID3D10Blob 구조체를 가리키는 포인터
- ppErrorMsgs: 컴파일 오류 시 오류 메시지을 담을 ID3d10Blog 구조체를 가리키는 포인터
- pHResult: 비동기 컴파일 시 오류 코드를 조회하는 데 쓰인다. pPump에 NULL을 지정했다면 이 매개변수에도 NULL을 지정한다.

---
