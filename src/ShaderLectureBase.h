#pragma once
/*
シェーダ講座用クラス
DirectXの初期化・描画ループ、Winのメッセージループ
*/

#include"base/IncludeAll.h"
#include"base/WindowsBase.h"
#include"dx11/DirectX11Base.h"
#include"base/FPSCounter.h"
#include"base/Camera.h"

class ShaderLectureBase{
protected:
	// デバイス
	DXDEVICE*	m_pDevice;
	DXCONTEXT*	m_pContext;
	DXSC*		m_pSwapChain;

	// オブジェクトポインタ
	unique_ptr<WindowsBase>			m_pWinBase;
	unique_ptr<DirectX11Base>		m_pDxBase;
	unique_ptr<DebugCommentManager>	m_pDebugComM;
	unique_ptr<Input>				m_pInput;
	unique_ptr<FPSCounter>			m_pFPSCount;
	unique_ptr<Camera>				m_pCamera;

	// マルチスレッド制御
	mutex mtx_GamaLoop;
	bool EndFlag = false;

	// 描画ターゲット
	ID3D11RenderTargetView*	m_pRenderTargetView = nullptr;	// 最終描画ターゲット
	ID3D11Texture2D*		m_pDepthStencil = nullptr;
	ID3D11DepthStencilView*	m_pDepthStencilView = nullptr;

public:
	ShaderLectureBase();
	virtual ~ShaderLectureBase();
	void Run(HINSTANCE hinst, int Cmd);


private:  protected:
	// BaseClassだけが呼ぶサブ関数
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

	// MAINclassで宣言する関数
	virtual void Init(){}
	virtual void Move(){}
	virtual void SetStateToShader(){}
	virtual void SetDataToShader(){}
	virtual void Draw(){}

};