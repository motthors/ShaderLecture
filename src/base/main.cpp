#include"../ShaderLectureMAIN.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow){

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �A�v���P�[�V�����̏�����
	ShaderLectureMAIN RM;
	RM.Run(hInstance,nCmdShow);

	return 0;
}