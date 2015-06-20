#ifndef _FPS_H_
#define _FPS_H_

#include"IncludeAll.h"
#include"DebugCommentManager.h"
#include"Input.h"

class FPSCounter{
	Input* pInput;

	DWORD mStartTime;       //測定開始時刻
	DWORD mCount;           //カウンタ
	float mFps;             //fps
	int N;					//平均を取るサンプル数
	int FPS;				//設定したFPS
	float ElapsedTime;

	DWORD nowtime,prevtime; //1フレーム分の時間
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