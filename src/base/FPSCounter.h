#ifndef _FPS_H_
#define _FPS_H_

#include"IncludeAll.h"
#include"DebugCommentManager.h"
#include"Input.h"

class FPSCounter{
	Input* pInput;

	DWORD mStartTime;       //����J�n����
	DWORD mCount;           //�J�E���^
	float mFps;             //fps
	int N;					//���ς����T���v����
	int FPS;				//�ݒ肵��FPS
	float ElapsedTime;

	DWORD nowtime,prevtime; //1�t���[�����̎���
	DWORD timesumtotal;
	DWORD PotentialPower;
	DWORD *timesave;

public:
	FPSCounter();
	~FPSCounter();
	void SetStdFPS(DWORD number);
	void SetVcon(Input* p){pInput=p;}

	float GetPrevElapsedTime();

	void Start();
	bool Update();
	void Draw(DebugCommentManager* pDebM);
	void Wait();

	void Wait2();
};


#endif