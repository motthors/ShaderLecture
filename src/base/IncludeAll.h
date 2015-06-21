#pragma once

#define DIRECTINPUT_VERSION 0x0800

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <share.h>

#include<d3d11.h>
#define DXDEVICE	ID3D11Device
#define DXCONTEXT	ID3D11DeviceContext
#define DXSC		IDXGISwapChain

#include <DirectXMath.h>
#include <dinput.h>

#include"define.h"

#include<thread>
#include<mutex>
#include <memory>
using namespace std;
#define ScopedLock(x) lock_guard<mutex> lock(x)


#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#include"../ErrorManager/ErrorManager.h"
#include"MyFunction.h"
