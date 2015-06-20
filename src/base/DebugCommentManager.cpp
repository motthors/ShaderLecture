#include"DebugCommentManager.h"


DebugCommentManager::DebugCommentManager()
{

	DivWindowNum = -1;
	Text = NULL;
	WindowDivX = 0;
	WindowDivY = 0;
}

DebugCommentManager::~DebugCommentManager()
{
	FOR(DivWindowNum* DivWindowNum)SAFE_DELETEARRAY(Text[i]);
	SAFE_DELETEARRAY(Text);
}

void DebugCommentManager::SetDeviceContext(DXDEVICE* pDevice, DXCONTEXT* pContext)
{
	m_pSpriteBatch = make_unique<SpriteBatch>(pContext);
	m_pSpriteFont = make_unique<SpriteFont>(pDevice, _T("sf.spritefont"));
}

void DebugCommentManager::SetDivideNum(int divnum, int WindowX, int WindowY)
{
	if(divnum == DivWindowNum)return;

	FOR(DivWindowNum)SAFE_DELETEARRAY(Text[i]);
	SAFE_DELETEARRAY(Text);
	DivWindowNum = divnum;
	Text = new TCHAR* [DivWindowNum * DivWindowNum];
	FOR(DivWindowNum * DivWindowNum)Text[i] = new TCHAR [TEXT_BOX_SIZE];

	WindowDivX = WindowX / divnum;
	WindowDivY = WindowY / divnum;

	Reset();
}

void DebugCommentManager::Reset()
{
	index = 1;
	FOR(DivWindowNum*DivWindowNum)Text[i][0]=0;
}

//void DebugCommentManager::BeginSet()

void DebugCommentManager::Set(TCHAR *src, int idx)
{
	if(idx != -1)
	{
		_tcscat_s(Text[idx],TEXT_BOX_SIZE,src);
	}
	else
	{
		_tcscat_s(Text[index],TEXT_BOX_SIZE,src);
	}
}

void DebugCommentManager::End()
{
	++index;
}

void DebugCommentManager::Draw()
{
	m_pSpriteBatch->Begin();
	XMFLOAT2 pos;
	FOR(DivWindowNum*DivWindowNum)
	{
		if (Text[i][0] == 0)continue;

		pos.x = (i % DivWindowNum)*WindowDivY;
		pos.y = (i / DivWindowNum)*WindowDivX;
		try
		{
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), Text[i], pos);
		}
		catch (...)
		{
			ALERT(_T("DebugCommentManager::Draw\nSpritefont‚Å—áŠO”­¶"));
		}
	}

	m_pSpriteBatch->End();
	Reset();
}
