#include"Input.h"

Input::Input(){

	pDI = NULL;
	pDIKeyBoard= NULL;
	for(int i=0;i<PadNum;++i)
		pDIDev[i] = NULL;

	memset(JoyPadState, 0, sizeof(DIJOYSTATE2) * 2 * PadNum);
	memset(KeyState, 0, sizeof(BYTE) * 256 * 2);
	memset(MouseState, 0, sizeof(DIMOUSESTATE2)*2);

	m_iDetectJoypadCount	= 0;
	m_iSettingJoypad		= 0;

	StateIndex = TRUE;
}

void Input::Del(){
	
	for(int i=0;i<PadNum;++i){
		if(pDIDev[i])pDIDev[i]->Unacquire();
		RELEASE(pDIDev[i]);
	}
	pDIKeyBoard->Unacquire();
	RELEASE(pDIKeyBoard);
	RELEASE(pDI);

}

void Input::Init(HINSTANCE hInstance, HWND hwnd, DebugCommentManager* pdc)
{
	HRESULT hr;
	int ErrID;
	this->hWnd = hwnd;
	m_pDebComM = pdc;

	//オブジェクト作成
	if (FAILED(hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(LPVOID*)&pDI,
		NULL)))
	{
		ErrID = 1;
		goto ERR;
	}
	//キーボードデバイス作成
	if (FAILED(hr = CreateKeyBoard())){
		ErrID = 2;
		goto ERR;
	}
	//ゲームパッドデバイス作成
	if (FAILED(hr = CreateGamePad())){
		ErrID = 3;
		goto ERR;
	}
	//マウスデバイス作成
	if (FAILED(hr = CreateMouse())){
		ErrID = 4;
		goto ERR;
	}

	return;

ERR:
	ErrM.SetClassName(_T("Input::Init"));
	switch (ErrID){
	case 1:ErrM.SetErrorText(_T("DirectInput8Createが失敗"));break;
	case 2:ErrM.SetErrorText(_T("CreateKeyBoardが失敗"));break;
	case 3:ErrM.SetErrorText(_T("CreateGamePadが失敗"));break;
	case 4:ErrM.SetErrorText(_T("CreateMouseが失敗"));break;
	}
	ErrM.SetHResult(USER_ERROR);
	throw &ErrM;
}

////////////////////////////////////////////////

HRESULT Input::CreateKeyBoard(){
	HRESULT hr;

	//キーボードデバイス生成
	if(FAILED(hr = pDI->CreateDevice(GUID_SysKeyboard,&pDIKeyBoard, NULL))){
		return hr;
	}

	//キーボード設定
	//フォーマットの設定
	if(FAILED(hr = pDIKeyBoard->SetDataFormat(&c_dfDIKeyboard))){
		return hr;
	}
	//バッファサイズの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 1000;
	if(FAILED(hr = pDIKeyBoard->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph))){
		return hr;
	}
	//協調レベルの設定
	if(FAILED(hr = pDIKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))){
		return hr;
	}
	//制御開始
	pDIKeyBoard->Acquire();
	return DI_OK;
}


///////////////////////////////////////////////////

HRESULT Input::CreateGamePad(){
	HRESULT hr;
	//接続しているコントローラデバイス列挙
	if(FAILED(hr = pDI->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)EnumDevCallback,
		(LPVOID *)this,
		DIEDFL_ATTACHEDONLY)))
		return hr;

	//コントローラ設定
	m_iDetectJoypadCount = m_iDetectJoypadCount > PadNum ? PadNum : m_iDetectJoypadCount;
	for(int i = 0; i < m_iDetectJoypadCount; ++i)
	{
		//フォーマットの設定
		if(FAILED(hr = pDIDev[i]->SetDataFormat(&c_dfDIJoystick2))){
			return hr;
		}

		//協調レベルの設定
		if(FAILED(hr = pDIDev[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE  | DISCL_FOREGROUND))){
			return hr;
		}
		if(FAILED(hr = pDIDev[i]->EnumObjects(
			(LPDIENUMDEVICEOBJECTSCALLBACK)EnumObjectCallback, (VOID *)this, DIDFT_ALL)))
		{
			return hr;
		}
		m_iSettingJoypad++;
		//ジョイパッド制御開始
		pDIDev[i]->Acquire();
	}
	return TRUE;
}

BOOL CALLBACK Input::EnumDevCallback(LPDIDEVICEINSTANCE lpddi,LPVOID pvRef){
	Input *pThis = (Input*)pvRef;

	//デバイスの作成
	if(FAILED(pThis->pDI->CreateDevice(lpddi->guidInstance,
		&(pThis->pDIDev[pThis->m_iDetectJoypadCount]), NULL)))
	{
		return DIENUM_STOP;
	}
	//ジョイパッドの数を増加
	pThis->m_iDetectJoypadCount++;

	return DIENUM_CONTINUE;
}

BOOL CALLBACK Input::EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pCont){
	Input *pThis = (Input*)pCont;

	if(pDidIns->dwType & DIDFT_AXIS)
	{
		//軸のデッドゾーン設定 20%
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize		= sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow		= DIPH_BYID;
		diprop.diph.dwObj		= pDidIns->dwType;
		diprop.dwData			= 2000;
		if(FAILED(pThis->pDIDev[pThis->m_iSettingJoypad]->SetProperty(
			DIPROP_DEADZONE,&diprop.diph)))
		{
			return DIENUM_STOP;
		}
		
		//軸の最大値設定 1000
		DIPROPRANGE	diprg;
		ZeroMemory(&diprg, sizeof(DIPROPRANGE));
		diprg.diph.dwSize		= sizeof(DIPROPRANGE);	//全体のサイズ
		diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);	//ヘッダのサイズ
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pDidIns->dwType;
		diprg.lMin				= -1000;
		diprg.lMax				=  1000;
		pThis->pDIDev[pThis->m_iSettingJoypad]->SetProperty(DIPROP_RANGE, &diprg.diph);


		if(FAILED(pThis->pDIDev[pThis->m_iSettingJoypad]->SetProperty(
			DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}


///////////////////////////////////////////////////

HRESULT Input::CreateMouse(){
	HRESULT hr;
	if (FAILED(hr = pDI->CreateDevice(GUID_SysMouse, &pDIMouse, NULL))){
		return hr;
	}
	//データフォーマットの設定
	if (FAILED(hr = pDIMouse->SetDataFormat(&c_dfDIMouse2))){
		return -1;
	}

	//バッファサイズの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 1000;
	hr = pDIMouse->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if (FAILED(hr)) {
		return -1;
	}

	//協調モードの設定
	hr = pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		return -1;
	}

	//入力を許可する
	pDIMouse->Acquire();

	return S_OK;
}

///////////////////////////////////////////////////

void Input::Update(){

	StateIndex = (BYTE)!StateIndex;
	HRESULT hr;


	//マウス
	pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState[StateIndex]);
	hr = pDIMouse->Acquire();

	//キーボード
	pDIKeyBoard->GetDeviceState( sizeof(KeyState[StateIndex]), KeyState[StateIndex] );
	//do{
		hr = pDIKeyBoard->Acquire();
	//}while(hr == DIERR_INPUTLOST);

	//コントローラ
	for(int i = 0; i < m_iDetectJoypadCount; ++i){
		if(FAILED(pDIDev[i]->Poll())){
			//do{
				hr = pDIDev[i]->Acquire();
			//}while(hr == DIERR_INPUTLOST);

			ZeroMemory(&JoyPadState[i][StateIndex], sizeof(JoyPadState[i][StateIndex]));

			return;
		}
		hr = pDIDev[i]->GetDeviceState(sizeof(DIJOYSTATE2), &JoyPadState[i][StateIndex]);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////

BYTE Input::GetKey(int ID_DIK_xx){
	//IDは DIK_○ で定義されている
	return KeyState[StateIndex][ID_DIK_xx];
}
BYTE Input::GetKeyNow(int ID){
	//IDは DIK_○ で定義されている
	return (KeyState[StateIndex][ID]
			^KeyState[!StateIndex][ID])
				&KeyState[StateIndex][ID];
}
BYTE Input::GetButton(BYTE num, BYTE playerNum){
	return JoyPadState[playerNum][StateIndex].rgbButtons[num];
}
BYTE Input::GetButtonNow(BYTE num, BYTE playerNum){
	return (JoyPadState[playerNum][StateIndex].rgbButtons[num] 
			^JoyPadState[playerNum][!StateIndex].rgbButtons[num])
				&JoyPadState[playerNum][StateIndex].rgbButtons[num];
}

BYTE Input::GetCross(int flag, BYTE playerNum){
	return (4500 >= (JoyPadState[playerNum][StateIndex].rgdwPOV[0] - flag)%36000);
}
BYTE Input::GetCrossNow(int flag, BYTE playerNum){
	bool f1 = (4500 >= (JoyPadState[playerNum][StateIndex].rgdwPOV[0] - flag)%36000);
	bool f2 = (4500 <= (JoyPadState[playerNum][!StateIndex].rgdwPOV[0] - flag)%36000);
	return f1 & f2;
}
LONG Input::GetLY(BYTE playerNum){
	return JoyPadState[playerNum][StateIndex].lY;
}
LONG Input::GetLX(BYTE playerNum){
	return JoyPadState[playerNum][StateIndex].lX;
}
LONG Input::GetRY(BYTE playerNum){
	return JoyPadState[playerNum][StateIndex].lRz;
}
LONG Input::GetRX(BYTE playerNum){
	return JoyPadState[playerNum][StateIndex].lZ;
}

/////////////////////////////マウス
BYTE Input::GetMouseButton(int flag){ return MouseState[StateIndex].rgbButtons[flag]; }
BYTE Input::GetMouseButtonOn(int flag){ 
	return MouseState[StateIndex].rgbButtons[flag]
		&& !MouseState[!StateIndex].rgbButtons[flag];
}
BYTE Input::GetMouseButtonOff(int flag){
	return !MouseState[StateIndex].rgbButtons[flag]
		&& MouseState[!StateIndex].rgbButtons[flag];
}

LONG Input::GetMouse_X_Relative(){ return MouseState[StateIndex].lX; }
LONG Input::GetMouse_Y_Relative(){ return MouseState[StateIndex].lY; }
LONG Input::GetMouse_Wheel(){ return MouseState[StateIndex].lZ; }
void Input::GetMouse_Absolute(LONG* x, LONG* y){
	GetCursorPos(&MousePos);
	ScreenToClient(hWnd, &MousePos);
	*x = MousePos.x;
	*y = MousePos.y;
}


void Input::debugDraw()
{
	TCHAR text[400]={0};
	_stprintf_s(text,_T("Input\n")
					_T("lX:%d\n")
					_T("lY:%d\n")
					_T("lZ:%d\n")
					_T("Button:%d %d %d %d %d %d %d %d\n")
					_T("MousePos:%d %d\n"),
		MouseState[StateIndex].lY,				 
		MouseState[StateIndex].lX,
		MouseState[StateIndex].lZ,
		MouseState[StateIndex].rgbButtons[0]/128,
		MouseState[StateIndex].rgbButtons[1]/128,
		MouseState[StateIndex].rgbButtons[2]/128,
		(BYTE)GetMouseButtonOn(INPUT_MOUSE_LEFT),
		(BYTE)GetMouseButtonOn(INPUT_MOUSE_RIGHT),
		(BYTE)GetMouseButtonOn(INPUT_MOUSE_CENTER),
		(BYTE)GetMouseButtonOff(INPUT_MOUSE_LEFT),
		MouseState[StateIndex].rgbButtons[7],
		MousePos.x,
		MousePos.y
		);
	
	m_pDebComM->Set(text);
	m_pDebComM->End();
}
