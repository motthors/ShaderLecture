#include"ShaderLectureMAIN.h"

struct deleater_Release{
	void operator()(IUnknown* ptr){ RELEASE(ptr); }
};

ShaderLectureMAIN::ShaderLectureMAIN()
{
}

ShaderLectureMAIN::~ShaderLectureMAIN()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pIndexBuffer);

	RELEASE(m_pVertexShader);
	RELEASE(m_pPixelShader);
	RELEASE(m_pLayout);

	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);

	RELEASE(m_pRasterizerState);
	RELEASE(m_pTextureBlendState);
	RELEASE(m_pCommonDepthStencilState);
	RELEASE(m_pSamplerState);
	RELEASE(m_DepthState);
}


typedef struct{
	float x, y, z;		// 座標
	float u, v;			// UV座標
	// DWROD color;		// 色情報
} VERTEX;


void 
ShaderLectureMAIN::Init()
{
	VERTEX vtx[4];
	// 頂点座標
	vtx[0].x = -1.0f;	vtx[0].y = 1.0f;	vtx[0].z = 0.0f;
	vtx[1].x = 0.5f;	vtx[1].y = 0.5f;	vtx[1].z = 0.0f;
	vtx[2].x = -0.5f;	vtx[2].y = -0.5f;	vtx[2].z = 0.0f;
	vtx[3].x = 1.0f;	vtx[3].y = -1.0f;	vtx[3].z = 0.0f;
	// テクセル
	vtx[0].u = 0.0f + 0.5f * WINDOW_SIZE_W; vtx[0].v = 0.0f + 0.5f * WINDOW_SIZE_H;
	vtx[1].u = 1.0f + 0.5f * WINDOW_SIZE_W; vtx[1].v = 0.0f + 0.5f * WINDOW_SIZE_H;
	vtx[2].u = 0.0f + 0.5f * WINDOW_SIZE_W; vtx[2].v = 1.0f + 0.5f * WINDOW_SIZE_H;
	vtx[3].u = 1.0f + 0.5f * WINDOW_SIZE_W; vtx[3].v = 1.0f + 0.5f * WINDOW_SIZE_H;
	// 頂点バッファを作成
	m_pDxBase->CreateVertexBuffer(&m_pVertexBuffer, vtx, sizeof(vtx), 0);

	// インデックスバッファ作成
	USHORT Index[] = { 0, 1, 2, 3 };
	m_pDxBase->CreateIndexBuffer(&m_pIndexBuffer, Index, sizeof(Index), 0);

	// 入力レイアウトの作成
	D3D11_INPUT_ELEMENT_DESC m_pLayoutDesc[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//シェーダの作成
	CreateVertexShader(&m_pVertexShader, _T("src/shader/VS_normal.cso"),
		m_pLayoutDesc, sizeof(m_pLayoutDesc) / sizeof(m_pLayoutDesc[0]), &m_pLayout);
	CreatePixelShader(&m_pPixelShader, _T("src/shader/PS_RefTex.cso"));


	//定数バッファ作成
	m_pDxBase->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(m2DDWConstVS), D3D11_CPU_ACCESS_WRITE);
	m_pDxBase->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(m2DDWConstPS), D3D11_CPU_ACCESS_WRITE);

	// ビューポートの設定
	D3D11_VIEWPORT	vp;
	vp.Width = static_cast<float>(WINDOW_SIZE_W);
	vp.Height = static_cast<float>(WINDOW_SIZE_H);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1, &vp);

	// ステート作成
	HRESULT hr;

	/////////////////////////////////////////////////////
	// ブレンドステート
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pTextureBlendState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateBlendState"));
		ErrM.SetErrorText(_T("ブレンドステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	/////////////////////////////////////////////////////
	// ラスタライザステート
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.DepthClipEnable = TRUE;
	hr = m_pDevice->CreateRasterizerState(&rsDesc, &m_pRasterizerState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateRasterizerState"));
		ErrM.SetErrorText(_T("ラスタライザステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	/////////////////////////////////////////////////////
	// デプスステンシルステート -> ShaderBox へ移動しました
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//// Depth ON Less WriteOK////
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;

	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_DepthState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateDepthStencilState"));
		ErrM.SetErrorText(_T("デプスステンシルステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	/////////////////////////////////////////////////////
	// サンプラステート
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pDevice->CreateSamplerState(&samDesc, &m_pSamplerState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateSamplerState"));
		ErrM.SetErrorText(_T("サンプラステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	// レンダリングターゲットを設定する
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

}


void
ShaderLectureMAIN::Move()
{

}


void
ShaderLectureMAIN::SetDataToShader()
{
	// VSへ定数バッファ設定
	m_pDxBase->SetConstBuffer(m_pConstantBafferVS, reinterpret_cast<void*>(&m_ConstVS), sizeof(m_ConstVS));
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
	// PSへ定数バッファ設定
	m_pDxBase->SetConstBuffer(m_pConstantBafferPS, reinterpret_cast<void*>(&m_ConstPS), sizeof(m_ConstPS));
	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);

	//シェーダをセット
	m_pContext->IASetInputLayout(m_pLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Set State
	m_pContext->OMSetBlendState(m_pTextureBlendState, Colors::Black, 0xffffffff);
	m_pContext->RSSetState(m_pRasterizerState);
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);
	m_pContext->OMSetDepthStencilState(m_DepthState, 0);
}


void
ShaderLectureMAIN::Draw()
{

	///////////////////////////

	/////Draw
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_pContext->DrawIndexed(4, 0, 0);

	////////////////////////////
}
