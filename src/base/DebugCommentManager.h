#ifndef DEBUG_COMMENT_MANAGER_H_
#define DEBUG_COMMENT_MANAGER_H_
//////////////////////////////////////////
//�f�o�b�O�R�����g�\���N���X
//��ʂ�NxN�ɕ������A�����Ń}�X������U��`��
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

	//�J���Ă�}�X��T��
	int SearchFreeBox();
	// �R�����g�����Z�b�g
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