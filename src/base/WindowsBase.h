#ifndef _WINDOWS_BASE_H_
#define _WINDOWS_BASE_H_


#include"IncludeAll.h"


class WindowsBase{

	HWND hWnd;
	HINSTANCE hInstance;
	int nCmdShow;
	WNDCLASSEX wcex;


	bool flscflag; //fullscreenflag
	int windowsizew;
	int windowsizeh;
public:
	WindowsBase();
	~WindowsBase();

	void Init(HINSTANCE hInstance, int nCmdShow);
	//void ChangeWindowFullandWnd;
	void ShowWnd();
	HWND GetHWND(){return hWnd;};
	bool GetFlscflag(){return flscflag;};
	int GetWindowSizeW(){return windowsizew;};
	int GetWindowSizeH(){return windowsizeh;};
};


#endif