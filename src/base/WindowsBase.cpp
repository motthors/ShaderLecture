#include"WindowsBase.h"

WindowsBase::WindowsBase(){

	hWnd = 0;
	nCmdShow = 0;
	flscflag = 0;
	windowsizew = WINDOW_SIZE_W;
	windowsizeh = WINDOW_SIZE_H;

}

WindowsBase::~WindowsBase(){

}

void WindowsBase::Init(HINSTANCE hInst, int nCmd){

	hInstance = hInst;
	nCmdShow = nCmd;

	HCURSOR hcursor = LoadCursor(hInstance, IDC_ARROW);

	WNDCLASSEX wcextemp ={
		sizeof(WNDCLASSEX), 
		CS_HREDRAW | CS_VREDRAW, 
		WndProc, 
		0,
		0, 
		hInstance,
		NULL, //�A�C�R��
		hcursor, //�J�[�\��
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		(TCHAR*)_T(_GAMENAME),
		NULL//�^�X�N�o�[�Ȃǂɕ\������鏬�����A�C�R���̏������A�C�R���n���h��
	};
	wcex = wcextemp;
	
	if(!RegisterClassEx(&wcex))
	{
		ErrM.SetClassName(_T("WindowsBase::Init::RegisterClassEx"));
		ErrM.SetErrorText(_T("�E�B���h�E�ݒ�Ɏ��s"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	
	if(!(hWnd = CreateWindow(
	   _T(_GAMENAME),
	   _T(_GAMENAME), 
	   WS_CAPTION | WS_SYSMENU, 
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT,
       NULL,
	   NULL,
	   hInstance,
	   NULL)))
	   {
		   ErrM.SetClassName(_T("WindowsBase::Init::CreateWindow"));
		   ErrM.SetErrorText(_T("�E�B���h�E�쐬�Ɏ��s"));
		   ErrM.SetHResult(USER_ERROR);
		   throw &ErrM;
	   }

	//�E�B���h�E�T�C�Y�̒���
	RECT rc;
	SetRect(&rc, 0, 0, windowsizew, windowsizeh);
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, FALSE);
	SetWindowPos(hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE | SWP_NOZORDER);

}

void WindowsBase::ShowWnd(){
	ShowWindow(hWnd, nCmdShow);
}