//
// Created by adunstudio on 2018-01-06.
//

#pragma once

// 필요한 내부 라이브러리들을 Include 한다.
#include <cstddef>
#include <cstdio>
#include <cmath>
#include <cstring>

/* STL */
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* STL Container */
#include <vector>
#include <list>
#include <map>
#include <unordered_map>


// 비트를 반환한다.
#define BIT(x) (1 << x)

// 함수를 바인딩한다.
#define METHOD_1(x) std::bind(x, this, std::placeholders::_1)
#define METHOD(x)   METHOD_1(x)

// 캡쳐 람다식
#define LAMBDA(x) [&](){ x(); }
