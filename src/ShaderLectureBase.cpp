#include"ShaderLectureBase.h"


static bool InitWindow(HINSTANCE hInstance);

ShaderLectureBase::ShaderLectureBase()
{

}

ShaderLectureBase::~ShaderLectureBase()
{
	RELEASE(m_pDepthStencilView);
	RELEASE(m_pDepthStencil);
	RELEASE(m_pRenderTargetView);

	RELEASE(m_pGridBuffer);
	RELEASE(m_pGridIndexBuffer);
	RELEASE(m_pGridLayout);
	RELEASE(m_pGridVShader);
	RELEASE(m_pConstantBafferVSG);
	RELEASE(m_pGridRS);

}

void ShaderLectureBase::Run(HINSTANCE hinst, int nCmd)
{
	try
	{
		m_pWinBase = make_unique<WindowsBase>();
		m_pWinBase->Init(hinst, nCmd);
		
		m_pDxBase = make_unique<DirectX11Base>();
		m_pDxBase->Init(m_pWinBase->GetHWND());
		
		m_pDebugComM = make_unique<DebugCommentManager>();
		m_pDebugComM->SetDeviceContext(m_pDxBase->GetDevice(), m_pDxBase->GetContext());
		m_pDebugComM->SetDivideNum(4, m_pWinBase->GetWindowSizeW(), m_pWinBase->GetWindowSizeH());
		
		m_pInput = make_unique<Input>();
		m_pInput->Init(hinst, m_pWinBase->GetHWND(), m_pDebugComM.get());
			
		m_pCamera = make_unique<Camera>();
		m_pCamera->SetViewPort(1, 0, WINDOW_SIZE_W, WINDOW_SIZE_H);
		m_pCamera->SetPers(XMConvertToRadians(65), 0.01f, 1000.0f);

		m_pFPSCount = make_unique<FPSCounter>();
		m_pFPSCount->SetStdFPS(60);
		m_pFPSCount->SetVcon(m_pInput.get());

		m_pWinBase->ShowWnd();
		m_pFPSCount->Start();

		subInit();
		Init();

		//Create threads
		thread thr_GameLoop(bind(&ShaderLectureBase::GameLoop, this));

		MSG msg;
		// ���b�Z�[�W ���[�v
		while (0 < GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		{ // ���b�Z�[�W���[�v�𔲂�����v���O�����I��
			ScopedLock(mtx_GamaLoop);
			EndFlag = true;
		}


		thr_GameLoop.join();
	}
	catch (ErrorManager* p)
	{
		//�G���[�����I������
		p->Alert();
		return;
	}
	
	return;
}



void ShaderLectureBase::GameLoop()
{
	try{
		while (1)
		{
			m_pFPSCount->Update();
			m_pFPSCount->Draw(m_pDebugComM.get());

			m_pInput->Update();
			m_pInput->debugDraw();
			subMove();
			Move();
			BeginDraw();
			SetDataToShader();
			Draw();
			subDraw();
			m_pDebugComM->Draw();
			EndDraw();
			m_pFPSCount->Wait();

			{ // �I������t���O�m�F
				ScopedLock(mtx_GamaLoop);
				if (EndFlag)break;
			}
		}
	}
	catch (ErrorManager* p)
	{
		p->Alert();
		SendMessage(m_pWinBase->GetHWND(), WM_CLOSE, 0, 0);
	}
}


void ShaderLectureBase::CreateVertexShader(
	ID3D11VertexShader** OutVS,
	TCHAR* csoName,
	D3D11_INPUT_ELEMENT_DESC* inLayout,
	int LayoutNum,
	ID3D11InputLayout** outLayout
	)
{
	HRESULT hr;

	// �o�C�i���V�F�[�_�[�t�@�C����ǂݍ���
	FILE* fp = _tfopen(csoName, _T("rb"));
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::_tfopen"));
		ErrM.SetErrorText(_T("cso�t�@�C���ǂݍ��݂Ɏ��s"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);
	fclose(fp);

	//���̎��_�ŃR���p�C����̃o�C�i����cso_data�Ɋi�[����Ă���//

	// ���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬//
	hr = m_pDxBase->GetDevice()->CreateVertexShader(cso_data, cso_sz, NULL, OutVS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::CreateVertexShader"));
		ErrM.SetErrorText(_T("���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	// ���͒��_�������쐬����
	hr = m_pDxBase->GetDevice()->CreateInputLayout(inLayout, LayoutNum, cso_data, cso_sz, outLayout);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::CreateInputLayout"));
		ErrM.SetErrorText(_T("���̓��C�A�E�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	delete[] cso_data;
}

void ShaderLectureBase::CreatePixelShader(
	ID3D11PixelShader** OutPS,
	TCHAR* csoName
	)
{
	// �o�C�i���t�@�C����ǂݍ���
	FILE* fp = _tfopen(csoName, _T("rb"));
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("ShaderBox::CreatePixelShader::_tfopen"));
		ErrM.SetErrorText(_T("cso�t�@�C���ǂݍ��݂Ɏ��s"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);
	fclose(fp);

	//���̎��_�ŃR���p�C����̃o�C�i����cso_data�Ɋi�[����Ă���//

	// ���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬//
	HRESULT hr = m_pDxBase->GetDevice()->CreatePixelShader(cso_data, cso_sz, NULL, OutPS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreatePixelShader::CreatePixelShader"));
		ErrM.SetErrorText(_T("�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	delete[] cso_data;
}


void ShaderLectureBase::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateRenderTarget::GetBuffer"));
		ErrM.SetErrorText(_T("�o�b�N�o�b�t�@�擾�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	hr = m_pDxBase->GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateRenderTarget::CreateRenderTargetView"));
		ErrM.SetErrorText(_T("�o�b�N�o�b�t�@RTV�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderLectureBase::CreateDepthStencil()
{
	// �[�x�o�b�t�@�e�N�X�`�����쐬����
	D3D11_TEXTURE2D_DESC	depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = WINDOW_SIZE_W;
	depthDesc.Height = WINDOW_SIZE_H;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	HRESULT	hr = m_pDxBase->GetDevice()->CreateTexture2D(&depthDesc, NULL, &m_pDepthStencil);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateDepthStencil::CreateTexture2D"));
		ErrM.SetErrorText(_T("�[�x�o�b�t�@�e�N�X�`���쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	// �[�x�o�b�t�@�^�[�Q�b�g���쐬����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pDxBase->GetDevice()->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateDepthStencil::CreateDepthStencilView"));
		ErrM.SetErrorText(_T("�[�x�o�b�t�@�^�[�Q�b�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}

typedef struct{
	float x, y, z;		// ���W
} VERTEX;

void ShaderLectureBase::subInit()
{
	m_pDevice = m_pDxBase->GetDevice();
	m_pContext = m_pDxBase->GetContext();
	m_pSwapChain = m_pDxBase->GetSwapChain();

	CreateRenderTarget();
	CreateDepthStencil();

	// �⏕�O���b�h���_�f�[�^�쐬
	float arr[10];
	FOR(10)arr[i] = i - 5;
	VERTEX vtx[10*4];
	ZeroMemory(vtx, sizeof(VERTEX) * 10 * 4);
	// ���_���W
	FOR(10)
	{
		vtx[i].x		=  arr[i];	vtx[i].z		=  5.0f;
		vtx[i + 10].x	= -arr[i];	vtx[i + 10].z	= -5.0f;
		vtx[i + 20].x	=  5.0f;	vtx[i + 20].z	= -arr[i];
		vtx[i + 30].x	= -5.0f;	vtx[i + 30].z	=  arr[i];
	}
	// ���_�o�b�t�@���쐬
	m_pDxBase->CreateVertexBuffer(&m_pGridBuffer, vtx, sizeof(vtx), 0);
	// �C���f�b�N�X�o�b�t�@�쐬
	USHORT Index[] = { 
		0, 30, 
		1, 19, 
		2, 18,
		3,17,
		4,16,
		5,15,
		6,14,
		7,13,
		8,12,
		9,11,
		10,20,
		0,20,
		39,21,
		38,22,
		37,23,
		36,24,
		35,25,
		34,26,
		33,27,
		32,28,
		31,29,
		30,10
	};
	m_pDxBase->CreateIndexBuffer(&m_pGridIndexBuffer, Index, sizeof(Index), 0);

	// �O���b�h�p���C�A�E�g
	// ���̓��C�A�E�g�̍쐬
	D3D11_INPUT_ELEMENT_DESC m_pLayoutDesc[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//�V�F�[�_�̍쐬
	CreateVertexShader(&m_pGridVShader, _T("src/shader/VS_grid.cso"),
		m_pLayoutDesc, sizeof(m_pLayoutDesc) / sizeof(m_pLayoutDesc[0]), &m_pGridLayout);

	// �萔�o�b�t�@
	XMStoreFloat4x4(&m_ConstVSG.World, XMMatrixIdentity());
	m_pDxBase->CreateConstantBuffer(&m_pConstantBafferVSG, NULL, sizeof(m_ConstVSG), D3D11_CPU_ACCESS_WRITE);

	/////////////////////////////////////////////////////
	// ���X�^���C�U�X�e�[�g
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.AntialiasedLineEnable = true; // ����AA�L��
	rsDesc.DepthClipEnable = TRUE;
	HRESULT hr = m_pDevice->CreateRasterizerState(&rsDesc, &m_pGridRS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateRasterizerState"));
		ErrM.SetErrorText(_T("���X�^���C�U�X�e�[�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderLectureBase::subMove()
{
	float x, y;
	if (m_pInput->GetMouseButton(INPUT_MOUSE_LEFT))
	{
		y = 0.002f*(float)m_pInput->GetMouse_Y_Relative();
		x = 0.002f*(float)m_pInput->GetMouse_X_Relative();
		m_pCamera->SetCameraRotaX_outside(x);
		m_pCamera->SetCameraRotaY_outside(-y);
	}
	m_pCamera->ShaderLecture();
}

void ShaderLectureBase::subDraw()
{
	// VS�֒萔�o�b�t�@�ݒ� �s���]�u���ēo�^�킷�ꂸ��
	mat V = XMLoadFloat4x4(m_pCamera->GetView());
	mat P = XMLoadFloat4x4(m_pCamera->GetPers());
	V = XMMatrixTranspose(V);
	P = XMMatrixTranspose(P);
	XMStoreFloat4x4(&m_ConstVSG.View, V);
	XMStoreFloat4x4(&m_ConstVSG.Proj, P);
	m_pDxBase->SetConstBuffer(m_pConstantBafferVSG, reinterpret_cast<void*>(&m_ConstVSG), sizeof(m_ConstVSG));
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVSG);

	//�V�F�[�_���Z�b�g
	m_pContext->IASetInputLayout(m_pGridLayout);
	m_pContext->VSSetShader(m_pGridVShader, NULL, 0);

	/////Draw
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pGridBuffer, &stride, &offset);
	m_pContext->IASetIndexBuffer(m_pGridIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_pContext->DrawIndexed(44, 0, 0);
}


void ShaderLectureBase::BeginDraw()
{
	////begin draw
	// Clear the back buffer
	float	clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShaderLectureBase::EndDraw()
{
	m_pSwapChain->Present(0, 0);
}

