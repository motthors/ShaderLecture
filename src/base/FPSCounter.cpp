#include"FPSCounter.h"
#include<MMSystem.h>

#define TimeSaveMemoryNum 60

FPSCounter::FPSCounter()
{
	N=30;		//1秒60フレームに1回FPS表示値を更新
	FPS=60;		//固定するFPS

	mStartTime	= 0;
	nowtime		= 0;
	prevtime	= 0;

	mCount = -1;
	mFps = 0;

	timesumtotal=0;

	timesave=NULL;
	timesave = new DWORD[120];
	FOR(TimeSaveMemoryNum)
	{
		timesave[i] = 0;
	}
	ElapsedTime = 0;
}
FPSCounter::~FPSCounter()
{
	SAFE_DELETEARRAY(timesave);
	timesave=NULL;
}
void FPSCounter::SetStdFPS(DWORD num)
{
	FPS = num;
	//mCount = 0;
	//prevtime=0;
	//timesumtotal = 0;
	//timeBeginPeriod(1);
	//mStartTime = timeGetTime();
	//timeEndPeriod(1);
	//nowtime = mStartTime;
	//for(UINT i=0; i<TimeSaveMemoryNum; ++i)
	//{
	//	timesave[i] = 0;
	//}
}

float FPSCounter::GetPrevElapsedTime()
{
	return ElapsedTime;
}

void FPSCounter::Start()
{
	timeBeginPeriod(1);
	nowtime = timeGetTime();						
	timeEndPeriod(1);
	prevtime=nowtime;
}

bool FPSCounter::Update()
{
	mCount++;
	//if( pInput->GetKeyNow(DIK_LBRACKET) ) SetStdFPS( FPS+=1 );
	//else if( pInput->GetKeyNow(DIK_RBRACKET) ) SetStdFPS( FPS-=1 );
	if( mCount % N == 0 ) //Nフレーム目なら平均を計算する
	{
		//DWORD t = timeGetTime();
		//mFps = 1000.f/((t-mStartTime-1)/(float)N);
		mFps = 1000.f*TimeSaveMemoryNum/timesumtotal;
		//mFps = timesumtotal;
	}
	if( mCount == TimeSaveMemoryNum )
	{
		mCount=0;
		mStartTime = nowtime;
	}
	return true;
}

void FPSCounter::Draw(DebugCommentManager* pDebM)
{
	TCHAR txt[50];
	_stprintf_s(txt,50,_T("StdSetting : %d\n"),FPS);
	pDebM->Set(txt,0);
	_stprintf_s(txt,50,_T("NowFPS : %.4f\n"),mFps);
	pDebM->Set(txt,0);
	_stprintf_s(txt,50,_T("Potential : %.1f\n"),1000/(float)PotentialPower);
	pDebM->Set(txt,0);

	FOR(6)
	{
		FOR2(10)
		{
			_stprintf_s(txt,50,_T("%02d "),timesave[i*10+j]);
			pDebM->Set(txt,0);
		}
	pDebM->Set(_T("\n"),0);
	}
	
	pDebM->End();
}

void FPSCounter::Wait()
{
	//時間取得
	timeBeginPeriod(1); // 精度を上げる
	nowtime = timeGetTime();						
	timeEndPeriod(1); // 変更を戻す

	PotentialPower = nowtime - prevtime;
	DWORD TookTime = nowtime - mStartTime;	//かかった時間 //tooktime
	float waitTime = 1000.f*(mCount+1)/FPS - TookTime;	//待つべき時間
	if( waitTime > 0.f ){
		Sleep((DWORD)waitTime);	//待機
		
		timeBeginPeriod(1);	
		nowtime = timeGetTime();							
		timeEndPeriod(1);
	}

	//int div = mCount;//%FPS;
	timesumtotal -= timesave[mCount];		
	timesave[mCount] = nowtime - prevtime;//１周した時間を記録
	timesumtotal += nowtime - prevtime;
	prevtime = nowtime;
		
	ElapsedTime = 1000.f/timesumtotal;
	if(ElapsedTime>7.f)ElapsedTime=0.f;

}

void FPSCounter::Wait2()
{
	//時間取得
	timeBeginPeriod(1); // 精度を上げる
	nowtime = timeGetTime();						
	timeEndPeriod(1); // 変更を戻す

	PotentialPower = nowtime - prevtime;	//1Fにかかった時間
	float waitTime = 16.6f*60.f/(float)FPS;	//待つべき時間
	if( PotentialPower < waitTime ){
		Sleep(waitTime-PotentialPower);	//待機
		
		timeBeginPeriod(1);	
		nowtime = timeGetTime();							
		timeEndPeriod(1);
	}

	int div = mCount%FPS;
	timesumtotal -= timesave[div];		
	timesave[mCount] = nowtime - prevtime;//１周した時間を記録
	timesumtotal += nowtime - prevtime;
	prevtime = nowtime;
		
	ElapsedTime = timesumtotal*0.001f;
	if(ElapsedTime>7.f)ElapsedTime=0.f;

}