#pragma once

// 표준 헤더
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<tchar.h>
#include<memory>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<set>
#include<string>
#include<algorithm>
#include<ctime>
#include<cmath>
#include<format>
#include<assert.h>
#include<filesystem>
namespace fs = std::filesystem;

// My 헤더
#include "Defines.h"
#include "Vec2.h"
#include "Enums.h"
#include "TimeManager.h"
#include "GDISelector.h"

// 정적 라이브러리
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

// 각종 using
using std::vector;
using std::list;
using std::map;
using std::wstring;

// 각종 define값
#define PI 3.14159265358979f
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// 메모리 릭
#ifdef _DEBUG
#define _CREDBG_MAP_ALLOC
#include<cstdlib>
#include<crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


// 디버깅용 콘솔창
#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif
#include<iostream>
using std::cout;
using std::endl;

#undef max;
#undef min;