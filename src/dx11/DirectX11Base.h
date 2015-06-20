#ifndef _DIRECTX11_BASE_H_
#define _DIRECTX11_BASE_H_

#include"../base/IncludeAll.h"
//#include"../base/DirectX9Base.h"
#include<dxgidebug.h>

class DirectX11Base{

	HWND				m_hWnd;
	D3D_FEATURE_LEVEL	m_FeatureLevel;		// Direct3D 初期化後、実際に採用されたフィーチャーレベル
	DXDEVICE*			m_pDevice;			// Direct3D11 デバイス
	DXCONTEXT*			 m_pContext;			// Direct3D11 デバイスコンテキスト
	DXSC*				m_pSwapChain;			

	//設定制御フラグ
	BOOL MultisampleEnabled	= TRUE;		//マルチサンプル可能なだけ高くする

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

	///////////// Dx11の機能を使った補助関数 ////////////////
	// バッファ作成
	void CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);
	// 頂点バッファ作成
	void CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	// インデックスバッファ作成
	void CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	// 定数バッファ作成
	void CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	//定数バッファ更新
	void SetConstBuffer(ID3D11Buffer* pBuffer, void* pSrcStruct, size_t Size);
};


#endif