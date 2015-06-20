#pragma once
/*
�V�F�[�_�u���p�N���X
DirectX�̏������E�`�惋�[�v�AWin�̃��b�Z�[�W���[�v
*/

#include"base/IncludeAll.h"
#include"base/WindowsBase.h"
#include"dx11/DirectX11Base.h"
#include"base/FPSCounter.h"
#include"base/Camera.h"

class ShaderLectureBase{
protected:
	// �f�o�C�X
	DXDEVICE*	m_pDevice;
	DXCONTEXT*	m_pContext;
	DXSC*		m_pSwapChain;

	// �I�u�W�F�N�g�|�C���^
	unique_ptr<WindowsBase>			m_pWinBase;
	unique_ptr<DirectX11Base>		m_pDxBase;
	unique_ptr<DebugCommentManager>	m_pDebugComM;
	unique_ptr<Input>				m_pInput;
	unique_ptr<FPSCounter>			m_pFPSCount;
	unique_ptr<Camera>				m_pCamera;

	// �}���`�X���b�h����
	mutex mtx_GamaLoop;
	bool EndFlag = false;

	// �`��^�[�Q�b�g
	ID3D11RenderTargetView*	m_pRenderTargetView = nullptr;	// �ŏI�`��^�[�Q�b�g
	ID3D11Texture2D*		m_pDepthStencil = nullptr;
	ID3D11DepthStencilView*	m_pDepthStencilView = nullptr;

public:
	ShaderLectureBase();
	virtual ~ShaderLectureBase();
	void Run(HINSTANCE hinst, int Cmd);


private:  protected:
	// BaseClass�������ĂԃT�u�֐�
	void subInit();
	void subMove();
	void subDraw();

	void GameLoop();
	void CreateDepthStencil();
	void CreateRenderTarget();

	void BeginDraw();
	void EndDraw();

protected:
	void CreateVertexShader(ID3D11VertexShader** OutVS, TCHAR* csoName, D3D11_INPUT_ELEMENT_DESC* inLayout, int LayoutNum, ID3D11InputLayout** outLayout);
	void CreatePixelShader(ID3D11PixelShader** OutVS, TCHAR* csoName);

	// MAINclass�Ő錾����֐�
	virtual void Init(){}
	virtual void Move(){}
	virtual void SetStateToShader(){}
	virtual void SetDataToShader(){}
	virtual void Draw(){}

};