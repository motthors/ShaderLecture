#pragma once

#include"../base/define.h"
#include<tchar.h>
#include<Windows.h>

#define CLASS_NAME_NUM 100
#define ERROR_TEXT_NUM 200

#define UNINITIALIZED -1
#define USER_ERROR	-2

class ErrorManager{
	TCHAR *m_ClassName;	// エラーが発生したクラス名と関数名
	TCHAR *m_ErrorText;	// エラー内容
	TCHAR *m_AlertText;	// メッセージボックスに表示する文
	HRESULT m_hr = UNINITIALIZED;// HRESULT情報
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