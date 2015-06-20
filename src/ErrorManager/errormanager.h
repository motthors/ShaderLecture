#pragma once

#include"../base/define.h"
#include<tchar.h>
#include<Windows.h>

#define CLASS_NAME_NUM 100
#define ERROR_TEXT_NUM 200

#define UNINITIALIZED -1
#define USER_ERROR	-2

class ErrorManager{
	TCHAR *m_ClassName;	// �G���[�����������N���X���Ɗ֐���
	TCHAR *m_ErrorText;	// �G���[���e
	TCHAR *m_AlertText;	// ���b�Z�[�W�{�b�N�X�ɕ\�����镶
	HRESULT m_hr = UNINITIALIZED;// HRESULT���
	TCHAR* ConvertErrorCode();
public:
	ErrorManager();
	~ErrorManager();
	void SetErrorText(TCHAR* txt);
	void SetClassName(TCHAR* txt);
	void SetHResult(HRESULT hr){ m_hr = hr; }
	void Alert();
};


#ifdef ERRORMANAGERDEF
#define GLOBAL  
#else
#define GLOBAL extern
#endif

GLOBAL ErrorManager ErrM;