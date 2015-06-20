#ifndef _INPUT_H_
#define _INPUT_H_

#include"IncludeAll.h"
#include"DebugCommentManager.h"

#define INPUT_CROSS_UP		0
#define INPUT_CROSS_RIGHT	9000
#define INPUT_CROSS_DOWN	18000
#define INPUT_CROSS_LEFT	27000

#define INPUT_MOUSE_LEFT	0
#define INPUT_MOUSE_RIGHT	1
#define INPUT_MOUSE_CENTER	2

#define PadNum 1

class Input{
	DebugCommentManager*	m_pDebComM;

	HWND					hWnd;
	LPDIRECTINPUT8			pDI;
	LPDIRECTINPUTDEVICE8	pDIKeyBoard;
	BYTE					KeyState[2][256];
	LPDIRECTINPUTDEVICE8	pDIDev[PadNum];
	DIJOYSTATE2				JoyPadState[PadNum][2];
	LPDIRECTINPUTDEVICE8	pDIMouse;
	DIMOUSESTATE2			MouseState[2];
	POINT					MousePos;	//�}�E�X���W


	BYTE StateIndex;		// ��Ԃ̃C���f�b�N�X
	
	int m_iDetectKeyBoardCount;	//���o�����L�[�{�[�h�̐�
	int m_iDetectJoypadCount;	//���o�����W���C�p�b�h�̐�
	int m_iSettingJoypad;		//�ݒ蒆�̃W���C�p�b�h

	static BOOL CALLBACK	EnumDevCallback(LPDIDEVICEINSTANCE , LPVOID );
	static BOOL CALLBACK	EnumObjectCallback(DIDEVICEOBJECTINSTANCE*, void* );

public:

	Input();
	void Del();
	void Init(HINSTANCE hinstance, HWND hWnd, DebugCommentManager* pdc);
	HRESULT CreateKeyBoard();
	HRESULT CreateGamePad();
	HRESULT CreateMouse();

	void Update();

	BYTE GetKey(int ID);
	BYTE GetKeyNow(int ID);
	BYTE GetButton(BYTE ButtonNum, BYTE playerNum);
	BYTE GetButtonNow(BYTE ButtonNum, BYTE playerNum);
	BYTE GetCross(int crossFlag, BYTE playerNum);
	BYTE GetCrossNow(int crossFlag, BYTE playerNum);
	LONG GetLY(BYTE playerNum);
	LONG GetLX(BYTE playerNum);
	LONG GetRY(BYTE playerNum);
	LONG GetRX(BYTE playerNum);

	//�}�E�X///////
	BYTE GetMouseButton(int ButtonFlag_INPUT_MOUSE_x); //�����Ă��邩�ǂ���
	BYTE GetMouseButtonOn(int ButtonFlag_INPUT_MOUSE_x); //�������u��
	BYTE GetMouseButtonOff(int ButtonFlag_INPUT_MOUSE_x); //�������u��
	//�������u��
	LONG GetMouse_X_Relative();
	LONG GetMouse_Y_Relative();
	LONG GetMouse_Wheel();
	void GetMouse_Absolute(LONG* x, LONG* y);

	void debugDraw();
};

#endif
