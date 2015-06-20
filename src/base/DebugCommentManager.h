#ifndef DEBUG_COMMENT_MANAGER_H_
#define DEBUG_COMMENT_MANAGER_H_
//////////////////////////////////////////
//デバッグコメント表示クラス
//画面をNxNに分割し、自動でマスを割り振り描画
////////////////////////////////////////


#include"../base/IncludeAll.h"

#include"../DXTK/Src/SpriteBatch.h"
#include"../DXTK/Src/SpriteFont.h"

using namespace DirectX;

#define TEXT_BOX_SIZE 1000

class DebugCommentManager {

	//DXTK
	unique_ptr<SpriteBatch>	m_pSpriteBatch;
	unique_ptr<SpriteFont>	m_pSpriteFont;

	int DivWindowNum;
	TCHAR **Text;
	int index;

	int WindowDivX, WindowDivY;

	//開いてるマスを探す
	int SearchFreeBox();
	// コメントをリセット
	void Reset();

public:
	DebugCommentManager();
	~DebugCommentManager();

	void SetDeviceContext(DXDEVICE* pDevice, DXCONTEXT* pContext);
	void SetDivideNum(int divnum, int WindowX, int WindowY);

	//void BeginSet();
	void Set(TCHAR *src, int idx=-1);
	void End();

	void Draw();
};

#endif