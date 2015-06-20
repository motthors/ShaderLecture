#define ERRORMANAGERDEF
#include"ErrorManager.h"

#include<d3d9.h>
#include<d3d11.h>

ErrorManager::ErrorManager()
{
	m_ClassName = new TCHAR[CLASS_NAME_NUM];
	m_ErrorText = new TCHAR[ERROR_TEXT_NUM];
	m_ClassName[0] = 0;
	m_ErrorText[0] = 0;
}

ErrorManager::~ErrorManager()
{
	SAFE_DELETEARRAY(m_ClassName);
	SAFE_DELETEARRAY(m_ErrorText);
}

void ErrorManager::SetErrorText(TCHAR* txt)
{
	_tcscpy_s(m_ErrorText, ERROR_TEXT_NUM, txt);
}

void ErrorManager::SetClassName(TCHAR* txt)
{
	_tcscpy_s(m_ClassName, CLASS_NAME_NUM, txt);
}

void ErrorManager::Alert()
{
	m_AlertText = new TCHAR[600];
	_stprintf_s(m_AlertText, 600, _T("%s内で例外が出ました\n・%s\n「%s」"),
		m_ClassName,
		m_ErrorText,
		ConvertErrorCode()
		);
	MessageBox(nullptr, m_AlertText, _T("Alert!"), MB_OK);
	SAFE_DELETEARRAY(m_AlertText);
}


//////////////////// ↓ Error Code ↓ /////////////////

TCHAR* ErrorManager::ConvertErrorCode()
{
	switch (m_hr)
	{
	//Direct3D 11 のリターン コード
	//https://msdn.microsoft.com/ja-jp/library/ee416213(v=vs.85).aspx
	case D3D11_ERROR_FILE_NOT_FOUND	:
		return _T("D3D11_ERROR_FILE_NOT_FOUND：ファイルが見つかりませんでした。");
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		return _T("D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS：特定の種類のステート オブジェクトの一意のインスタンスが多すぎます。");
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		return _T("D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS：特定の種類のビュー オブジェクトの一意のインスタンスが多すぎます。");
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		return _T("D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD：リソースごとの ID3D11Device::CreateDeferredContext の呼び出しまたは ID3D11DeviceContext::FinishCommandList の呼び出しの後で最初に呼び出した ID3D11DeviceContext::Map が D3D11_MAP_WRITE_DISCARD ではありませんでした。");
	case D3DERR_INVALIDCALL:
		return _T("D3DERR_INVALIDCALL：メソッドの呼び出しが無効です。たとえば、メソッドのパラメーターが有効なポインターでない可能性があります。");
	case D3DERR_WASSTILLDRAWING:
		return _T("このサーフェスとの間で情報を転送している以前のビット演算が不完全です。");
	case D3DERR_OUTOFVIDEOMEMORY:
		return _T("Direct3D が処理を行うのに十分なディスプレイ メモリがない。");
	case E_FAIL:
		return _T("E_FAIL：デバッグ レイヤーを有効にしてデバイスを作成しようとしましたが、該当するレイヤーがインストールされていません。");
	case E_INVALIDARG:
		return _T("E_INVALIDARG：戻り関数に無効なパラメーターが渡されました。");
	case E_OUTOFMEMORY:
		return _T("E_OUTOFMEMORY：Direct3D が呼び出しを完了するうえで十分なメモリーを割り当てることができませんでした。");
	case S_FALSE:
		//return _T("S_FALSE：正常に処理されたものの、非標準の完了を示す代替成功値です(正確な意味はコンテキストによって異なります)。");
		return _T("S_FALSE：テスト：パラーメータは全て正常です。");
	case S_OK:
		return _T("S_OK：エラーは発生していません。");

	//DXGI_ERROR
	//https://msdn.microsoft.com/en-us/library/windows/desktop/bb509553(v=vs.85).aspx
	case DXGI_ERROR_INVALID_CALL:
		return _T("関数の引数パラメータが無効です。");


	case UNINITIALIZED:
		ALERT(_T("エラーマネージャを使用せずに例外が投げられました。"));
		return _T("不明なエラーです。");
	case USER_ERROR:
		return _T("USER_ERRORが投げられました。");
	default :
		ALERT(_T("リストにないエラーコードが発生中"));
		return _T("不明なエラーです。");
	}
}
