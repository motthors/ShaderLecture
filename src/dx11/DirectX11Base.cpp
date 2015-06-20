#include"DirectX11Base.h"

DirectX11Base::DirectX11Base()
{
	m_hWnd			= 0;
	m_pDevice		= nullptr;
	m_pContext		= nullptr;
	m_pSwapChain	= nullptr;
}

DirectX11Base::~DirectX11Base()
{
	RELEASE(m_pSwapChain);
	RELEASE(m_pContext);
	RELEASE(m_pDevice);

#ifdef _DEBUG
	//if(m_pDebug)m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	RELEASE(m_pDebug);
#endif
}


// デバイスの生成
HRESULT DirectX11Base::CreateDeviceAndSwapChain()
{
	HRESULT hr = E_FAIL;

	///////////////////////////////////////////////////////////
	// スワップチェインの設定
	
	// DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// DXGI_MODE_DESC
	sd.BufferDesc.Width = WINDOW_SIZE_W;　
	sd.BufferDesc.Height = WINDOW_SIZE_H;
	sd.BufferDesc.RefreshRate.Numerator = 60;		//DXGI_RATIONAL リフレッシュレートの最大値
	sd.BufferDesc.RefreshRate.Denominator = 1;		//DXGI_RATIONAL リフレッシュレートの最小値
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// DXGI_MODE_SCANLINE_ORDER
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// DXGI_MODE_SCALING  スケーリング モード
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファにレンダリング可能にする。バックバッファのサーフェス使用法および CPUアクセスオプションを表す DXGI_USAGE 列挙型
	sd.BufferCount = 1;			//スワップ チェーンのバッファー数を表す値。
	sd.OutputWindow = m_hWnd;	// 出力ウィンドウへの HWND ハンドル。このメンバーを NULL にはできない。
	sd.Windowed = TRUE;			// 出力がウィンドウ モードの場合は TRUE。それ以外の場合は FALSE。
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// バックバッファの内容を保持しない。 サーフェスのスワップ処理後にバックバッファの内容を保持するか。
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// フルスクリーンモードに切り替えると、アプリケーション ウィンドウのサイズに合わせて解像度が変更される。 スワップ チェーンの動作のオプション。

	// マルチサンプリングの設定
	// マルチサンプルを使用しない場合の設定
	sd.SampleDesc.Count = 1;	// ピクセル単位のマルチサンプリングの数
	sd.SampleDesc.Quality = 0;	// イメージの品質レベル

	//int startMultisample;
	//if (MultisampleEnabled == TRUE) startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;  // サンプリング数が最大のものを採用する
	//else							startMultisample = 1;		// マルチサンプルを無効にする。


	///////////////////////////////////////////////////////////
	// デバイスの設定


	// デバイス作成時に使用するビデオカードの性能一覧
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};
	// featureCntを1にすればDx11でしか起動しない
	int featureCnt = sizeof(FeatureLevel) / sizeof(D3D_FEATURE_LEVEL);

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグコンパイルの場合、デバッグレイヤーを有効にする。
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlag = 0;
#endif

	////////////////////////////////////////////////////
	// デバイス作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,							// 使用するアダプターを設定。NULLの場合はデフォルトのアダプター。
		D3D_DRIVER_TYPE_HARDWARE,		// D3D_DRIVER_TYPEのいずれか。ドライバーの種類。pAdapterが NULL 以外の場合は、D3D_DRIVER_TYPE_UNKNOWNを指定する。
		NULL,							// ソフトウェアラスタライザを実装するDLLへのハンドル。D3D_DRIVER_TYPE を D3D_DRIVER_TYPE_SOFTWARE に設定している場合は NULL にできない。
		createDeviceFlag,				// D3D11_CREATE_DEVICE_FLAGの組み合わせ。デバイスの作成時に使用されるパラメータ。
		FeatureLevel,					// D3D_FEATURE_LEVELのポインタ
		featureCnt,						// D3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,				// DirectX SDKのバージョン。この値は固定。
		&sd,							// スワップチェイン設定DXGI_SWAP_CHAIN_DESCのポインタ
		&m_pSwapChain,					// スワップチェインポインタ
		&m_pDevice,						// 初期化されたデバイス
		&m_FeatureLevel,				// 採用されたフィーチャーレベル
		&m_pContext);					// 初期化されたデバイスコンテキスト

	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DirectX11Base::D3D11CreateDeviceAndSwapChain"));
		ErrM.SetErrorText(_T("デバイスの初期化に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}


#ifdef _DEBUG
	// デバッグ表示用
	hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_pDebug));
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DirectX11Base::CreateDeviceAndSwapChain::QueryInterface"));
		ErrM.SetErrorText(_T("デバッグデバイス作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
#endif

	return S_OK;
}


void DirectX11Base::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// デバイス生成
	CreateDeviceAndSwapChain();
}


void DirectX11Base::CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag)
{
	HRESULT hr;

	// バッファー リソース D3D11_BUFFER_DESC
	D3D11_BUFFER_DESC BufferDesc;

	// サブリソース D3D11_SUBRESOURCE_DATA
	D3D11_SUBRESOURCE_DATA* resource = NULL;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = 0;

	switch (CPUAccessFlag)
	{
	// CPUアクセスを許可しない
	case 0:
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = CPUAccessFlag;
		break;
	// CPUアクセスを許可する
	default:
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = CPUAccessFlag;
		break;
	}

	// 初期値を設定する
	if (pData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	// バッファの設定
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = size;						// バッファサイズ
	BufferDesc.Usage = Usage;							// リソース使用法を特定する
	BufferDesc.BindFlags = BindFlag;					// バッファの種類
	BufferDesc.CPUAccessFlags = CPUAccessFlags;			// CPU アクセス
	BufferDesc.MiscFlags = 0;							// その他のフラグも設定しない

	// バッファを作成する
	hr = m_pDevice->CreateBuffer(&BufferDesc, resource, pBuffer);
	if (FAILED(hr))
	{
		SAFE_DELETE(resource);
		ErrM.SetClassName(_T("DirectX11Base::CreateBuffer::CreateBuffer"));
		ErrM.SetErrorText(_T("バッファ作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	SAFE_DELETE(resource);
}

// 頂点バッファ作成
void DirectX11Base::CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_VERTEX_BUFFER);
}

// インデックスバッファ作成
void DirectX11Base::CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_INDEX_BUFFER);
}

// 定数バッファ作成
void DirectX11Base::CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_CONSTANT_BUFFER);
}

//定数バッファ更新
void DirectX11Base::SetConstBuffer(ID3D11Buffer* pBuffer, void* pSrcStruct, size_t Size)
{
	// コンスタントバッファを変更する
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_pContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr))
	{
		void* param = resource.pData;
		memcpy_s(param, Size, pSrcStruct, Size);
		m_pContext->Unmap(pBuffer, 0);
	}
	else
	{
		ErrM.SetClassName(_T("DirectX11Base::SetConstBuffer::Map"));
		ErrM.SetErrorText(_T("定数バッファ更新に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}