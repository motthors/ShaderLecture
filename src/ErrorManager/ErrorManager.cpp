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
	_stprintf_s(m_AlertText, 600, _T("%s���ŗ�O���o�܂���\n�E%s\n�u%s�v"),
		m_ClassName,
		m_ErrorText,
		ConvertErrorCode()
		);
	MessageBox(nullptr, m_AlertText, _T("Alert!"), MB_OK);
	SAFE_DELETEARRAY(m_AlertText);
}


//////////////////// �� Error Code �� /////////////////

TCHAR* ErrorManager::ConvertErrorCode()
{
	switch (m_hr)
	{
	//Direct3D 11 �̃��^�[�� �R�[�h
	//https://msdn.microsoft.com/ja-jp/library/ee416213(v=vs.85).aspx
	case D3D11_ERROR_FILE_NOT_FOUND	:
		return _T("D3D11_ERROR_FILE_NOT_FOUND�F�t�@�C����������܂���ł����B");
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		return _T("D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS�F����̎�ނ̃X�e�[�g �I�u�W�F�N�g�̈�ӂ̃C���X�^���X���������܂��B");
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		return _T("D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS�F����̎�ނ̃r���[ �I�u�W�F�N�g�̈�ӂ̃C���X�^���X���������܂��B");
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		return _T("D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD�F���\�[�X���Ƃ� ID3D11Device::CreateDeferredContext �̌Ăяo���܂��� ID3D11DeviceContext::FinishCommandList �̌Ăяo���̌�ōŏ��ɌĂяo���� ID3D11DeviceContext::Map �� D3D11_MAP_WRITE_DISCARD �ł͂���܂���ł����B");
	case D3DERR_INVALIDCALL:
		return _T("D3DERR_INVALIDCALL�F���\�b�h�̌Ăяo���������ł��B���Ƃ��΁A���\�b�h�̃p�����[�^�[���L���ȃ|�C���^�[�łȂ��\��������܂��B");
	case D3DERR_WASSTILLDRAWING:
		return _T("���̃T�[�t�F�X�Ƃ̊Ԃŏ���]�����Ă���ȑO�̃r�b�g���Z���s���S�ł��B");
	case D3DERR_OUTOFVIDEOMEMORY:
		return _T("Direct3D ���������s���̂ɏ\���ȃf�B�X�v���C ���������Ȃ��B");
	case E_FAIL:
		return _T("E_FAIL�F�f�o�b�O ���C���[��L���ɂ��ăf�o�C�X���쐬���悤�Ƃ��܂������A�Y�����郌�C���[���C���X�g�[������Ă��܂���B");
	case E_INVALIDARG:
		return _T("E_INVALIDARG�F�߂�֐��ɖ����ȃp�����[�^�[���n����܂����B");
	case E_OUTOFMEMORY:
		return _T("E_OUTOFMEMORY�FDirect3D ���Ăяo�����������邤���ŏ\���ȃ������[�����蓖�Ă邱�Ƃ��ł��܂���ł����B");
	case S_FALSE:
		//return _T("S_FALSE�F����ɏ������ꂽ���̂́A��W���̊�����������֐����l�ł�(���m�ȈӖ��̓R���e�L�X�g�ɂ���ĈقȂ�܂�)�B");
		return _T("S_FALSE�F�e�X�g�F�p���[���[�^�͑S�Đ���ł��B");
	case S_OK:
		return _T("S_OK�F�G���[�͔������Ă��܂���B");

	//DXGI_ERROR
	//https://msdn.microsoft.com/en-us/library/windows/desktop/bb509553(v=vs.85).aspx
	case DXGI_ERROR_INVALID_CALL:
		return _T("�֐��̈����p�����[�^�������ł��B");


	case UNINITIALIZED:
		ALERT(_T("�G���[�}�l�[�W�����g�p�����ɗ�O���������܂����B"));
		return _T("�s���ȃG���[�ł��B");
	case USER_ERROR:
		return _T("USER_ERROR���������܂����B");
	default :
		ALERT(_T("���X�g�ɂȂ��G���[�R�[�h��������"));
		return _T("�s���ȃG���[�ł��B");
	}
}
