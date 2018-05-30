#pragma once

#ifdef _MSC_VER 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib") 
#endif

/* DirectX 11 */
#include <d3d11.h>                  // Direct3D 11을 사용하기 위한 라이브러리
#include <d3dcompiler.h>            // Shader 코드 컴파일을 위한 라이브러리

enum DIMENSION { D2 = 0, D3 = 1};