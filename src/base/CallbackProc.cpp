#include"IncludeAll.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT mes, WPARAM wparam, LPARAM lparam)
{
	switch(mes){
	case WM_DESTROY: 
		PostQuitMessage(0); return 0;
	case WM_KEYDOWN:
		switch(wparam){
		case VK_ESCAPE:
			//PostQuitMessage(0);
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		case VK_LWIN:
		case VK_RWIN:
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, mes, wparam, lparam);
}
