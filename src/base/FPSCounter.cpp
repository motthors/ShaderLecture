#include"FPSCounter.h"
#include<MMSystem.h>

#define TimeSaveMemoryNum 60

FPSCounter::FPSCounter()
{
	N=30;		//1�b60�t���[����1��FPS�\���l���X�V
	FPS=60;		//�Œ肷��FPS

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
	if( mCount % N == 0 ) //N�t���[���ڂȂ畽�ς��v�Z����
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
	//���Ԏ擾
	timeBeginPeriod(1); // ���x���グ��
	nowtime = timeGetTime();						
	timeEndPeriod(1); // �ύX��߂�

	PotentialPower = nowtime - prevtime;
	DWORD TookTime = nowtime - mStartTime;	//������������ //tooktime
	float waitTime = 1000.f*(mCount+1)/FPS - TookTime;	//�҂ׂ�����
	if( waitTime > 0.f ){
		Sleep((DWORD)waitTime);	//�ҋ@
		
		timeBeginPeriod(1);	
		nowtime = timeGetTime();							
		timeEndPeriod(1);
	}

	//int div = mCount;//%FPS;
	timesumtotal -= timesave[mCount];		
	timesave[mCount] = nowtime - prevtime;//�P���������Ԃ��L�^
	timesumtotal += nowtime - prevtime;
	prevtime = nowtime;
		
	ElapsedTime = 1000.f/timesumtotal;
	if(ElapsedTime>7.f)ElapsedTime=0.f;

}

void FPSCounter::Wait2()
{
	//���Ԏ擾
	timeBeginPeriod(1); // ���x���グ��
	nowtime = timeGetTime();						
	timeEndPeriod(1); // �ύX��߂�

	PotentialPower = nowtime - prevtime;	//1F�ɂ�����������
	float waitTime = 16.6f*60.f/(float)FPS;	//�҂ׂ�����
	if( PotentialPower < waitTime ){
		Sleep(waitTime-PotentialPower);	//�ҋ@
		
		timeBeginPeriod(1);	
		nowtime = timeGetTime();							
		timeEndPeriod(1);
	}

	int div = mCount%FPS;
	timesumtotal -= timesave[div];		
	timesave[mCount] = nowtime - prevtime;//�P���������Ԃ��L�^
	timesumtotal += nowtime - prevtime;
	prevtime = nowtime;
		
	ElapsedTime = timesumtotal*0.001f;
	if(ElapsedTime>7.f)ElapsedTime=0.f;

}