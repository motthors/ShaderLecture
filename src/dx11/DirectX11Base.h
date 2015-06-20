#ifndef _DIRECTX11_BASE_H_
#define _DIRECTX11_BASE_H_

#include"../base/IncludeAll.h"
//#include"../base/DirectX9Base.h"
#include<dxgidebug.h>

class DirectX11Base{

	HWND				m_hWnd;
	D3D_FEATURE_LEVEL	m_FeatureLevel;		// Direct3D ��������A���ۂɍ̗p���ꂽ�t�B�[�`���[���x��
	DXDEVICE*			m_pDevice;			// Direct3D11 �f�o�C�X
	DXCONTEXT*			 m_pContext;			// Direct3D11 �f�o�C�X�R���e�L�X�g
	DXSC*				m_pSwapChain;			

	//�ݒ萧��t���O
	BOOL MultisampleEnabled	= TRUE;		//�}���`�T���v���\�Ȃ�����������

	HRESULT CreateDeviceAndSwapChain();

public:
	DirectX11Base();
	~DirectX11Base();

	void Init(HWND hWnd);

	DXDEVICE*	GetDevice(){ return m_pDevice; }
	DXCONTEXT*	GetContext(){ return m_pContext; }
	DXSC*		GetSwapChain(){ return m_pSwapChain; }

#ifdef _DEBUG
	ID3D11Debug* m_pDebug = 0;
#endif

	///////////// Dx11�̋@�\���g�����⏕�֐� ////////////////
	// �o�b�t�@�쐬
	void CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);
	// ���_�o�b�t�@�쐬
	void CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	// �C���f�b�N�X�o�b�t�@�쐬
	void CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	// �萔�o�b�t�@�쐬
	void CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	//�萔�o�b�t�@�X�V
	void SetConstBuffer(ID3D11Buffer* pBuffer, void* pSrcStruct, size_t Size);
};


#endif