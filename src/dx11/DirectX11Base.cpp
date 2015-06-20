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


// �f�o�C�X�̐���
HRESULT DirectX11Base::CreateDeviceAndSwapChain()
{
	HRESULT hr = E_FAIL;

	///////////////////////////////////////////////////////////
	// �X���b�v�`�F�C���̐ݒ�
	
	// DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// DXGI_MODE_DESC
	sd.BufferDesc.Width = WINDOW_SIZE_W;�@
	sd.BufferDesc.Height = WINDOW_SIZE_H;
	sd.BufferDesc.RefreshRate.Numerator = 60;		//DXGI_RATIONAL ���t���b�V�����[�g�̍ő�l
	sd.BufferDesc.RefreshRate.Denominator = 1;		//DXGI_RATIONAL ���t���b�V�����[�g�̍ŏ��l
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// DXGI_MODE_SCANLINE_ORDER
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// DXGI_MODE_SCALING  �X�P�[�����O ���[�h
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�Ƀ����_�����O�\�ɂ���B�o�b�N�o�b�t�@�̃T�[�t�F�X�g�p�@����� CPU�A�N�Z�X�I�v�V������\�� DXGI_USAGE �񋓌^
	sd.BufferCount = 1;			//�X���b�v �`�F�[���̃o�b�t�@�[����\���l�B
	sd.OutputWindow = m_hWnd;	// �o�̓E�B���h�E�ւ� HWND �n���h���B���̃����o�[�� NULL �ɂ͂ł��Ȃ��B
	sd.Windowed = TRUE;			// �o�͂��E�B���h�E ���[�h�̏ꍇ�� TRUE�B����ȊO�̏ꍇ�� FALSE�B
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// �o�b�N�o�b�t�@�̓��e��ێ����Ȃ��B �T�[�t�F�X�̃X���b�v������Ƀo�b�N�o�b�t�@�̓��e��ێ����邩�B
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �t���X�N���[�����[�h�ɐ؂�ւ���ƁA�A�v���P�[�V���� �E�B���h�E�̃T�C�Y�ɍ��킹�ĉ𑜓x���ύX�����B �X���b�v �`�F�[���̓���̃I�v�V�����B

	// �}���`�T���v�����O�̐ݒ�
	// �}���`�T���v�����g�p���Ȃ��ꍇ�̐ݒ�
	sd.SampleDesc.Count = 1;	// �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	sd.SampleDesc.Quality = 0;	// �C���[�W�̕i�����x��

	//int startMultisample;
	//if (MultisampleEnabled == TRUE) startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;  // �T���v�����O�����ő�̂��̂��̗p����
	//else							startMultisample = 1;		// �}���`�T���v���𖳌��ɂ���B


	///////////////////////////////////////////////////////////
	// �f�o�C�X�̐ݒ�


	// �f�o�C�X�쐬���Ɏg�p����r�f�I�J�[�h�̐��\�ꗗ
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};
	// featureCnt��1�ɂ����Dx11�ł����N�����Ȃ�
	int featureCnt = sizeof(FeatureLevel) / sizeof(D3D_FEATURE_LEVEL);

#if defined(DEBUG) || defined(_DEBUG)
	// �f�o�b�O�R���p�C���̏ꍇ�A�f�o�b�O���C���[��L���ɂ���B
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlag = 0;
#endif

	////////////////////////////////////////////////////
	// �f�o�C�X�쐬
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,							// �g�p����A�_�v�^�[��ݒ�BNULL�̏ꍇ�̓f�t�H���g�̃A�_�v�^�[�B
		D3D_DRIVER_TYPE_HARDWARE,		// D3D_DRIVER_TYPE�̂����ꂩ�B�h���C�o�[�̎�ށBpAdapter�� NULL �ȊO�̏ꍇ�́AD3D_DRIVER_TYPE_UNKNOWN���w�肷��B
		NULL,							// �\�t�g�E�F�A���X�^���C�U����������DLL�ւ̃n���h���BD3D_DRIVER_TYPE �� D3D_DRIVER_TYPE_SOFTWARE �ɐݒ肵�Ă���ꍇ�� NULL �ɂł��Ȃ��B
		createDeviceFlag,				// D3D11_CREATE_DEVICE_FLAG�̑g�ݍ��킹�B�f�o�C�X�̍쐬���Ɏg�p�����p�����[�^�B
		FeatureLevel,					// D3D_FEATURE_LEVEL�̃|�C���^
		featureCnt,						// D3D_FEATURE_LEVEL�z��̗v�f��
		D3D11_SDK_VERSION,				// DirectX SDK�̃o�[�W�����B���̒l�͌Œ�B
		&sd,							// �X���b�v�`�F�C���ݒ�DXGI_SWAP_CHAIN_DESC�̃|�C���^
		&m_pSwapChain,					// �X���b�v�`�F�C���|�C���^
		&m_pDevice,						// ���������ꂽ�f�o�C�X
		&m_FeatureLevel,				// �̗p���ꂽ�t�B�[�`���[���x��
		&m_pContext);					// ���������ꂽ�f�o�C�X�R���e�L�X�g

	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DirectX11Base::D3D11CreateDeviceAndSwapChain"));
		ErrM.SetErrorText(_T("�f�o�C�X�̏������Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}


#ifdef _DEBUG
	// �f�o�b�O�\���p
	hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_pDebug));
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DirectX11Base::CreateDeviceAndSwapChain::QueryInterface"));
		ErrM.SetErrorText(_T("�f�o�b�O�f�o�C�X�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
#endif

	return S_OK;
}


void DirectX11Base::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// �f�o�C�X����
	CreateDeviceAndSwapChain();
}


void DirectX11Base::CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag)
{
	HRESULT hr;

	// �o�b�t�@�[ ���\�[�X D3D11_BUFFER_DESC
	D3D11_BUFFER_DESC BufferDesc;

	// �T�u���\�[�X D3D11_SUBRESOURCE_DATA
	D3D11_SUBRESOURCE_DATA* resource = NULL;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = 0;

	switch (CPUAccessFlag)
	{
	// CPU�A�N�Z�X�������Ȃ�
	case 0:
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = CPUAccessFlag;
		break;
	// CPU�A�N�Z�X��������
	default:
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = CPUAccessFlag;
		break;
	}

	// �����l��ݒ肷��
	if (pData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	// �o�b�t�@�̐ݒ�
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = size;						// �o�b�t�@�T�C�Y
	BufferDesc.Usage = Usage;							// ���\�[�X�g�p�@����肷��
	BufferDesc.BindFlags = BindFlag;					// �o�b�t�@�̎��
	BufferDesc.CPUAccessFlags = CPUAccessFlags;			// CPU �A�N�Z�X
	BufferDesc.MiscFlags = 0;							// ���̑��̃t���O���ݒ肵�Ȃ�

	// �o�b�t�@���쐬����
	hr = m_pDevice->CreateBuffer(&BufferDesc, resource, pBuffer);
	if (FAILED(hr))
	{
		SAFE_DELETE(resource);
		ErrM.SetClassName(_T("DirectX11Base::CreateBuffer::CreateBuffer"));
		ErrM.SetErrorText(_T("�o�b�t�@�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	SAFE_DELETE(resource);
}

// ���_�o�b�t�@�쐬
void DirectX11Base::CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_VERTEX_BUFFER);
}

// �C���f�b�N�X�o�b�t�@�쐬
void DirectX11Base::CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_INDEX_BUFFER);
}

// �萔�o�b�t�@�쐬
void DirectX11Base::CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_CONSTANT_BUFFER);
}

//�萔�o�b�t�@�X�V
void DirectX11Base::SetConstBuffer(ID3D11Buffer* pBuffer, void* pSrcStruct, size_t Size)
{
	// �R���X�^���g�o�b�t�@��ύX����
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
		ErrM.SetErrorText(_T("�萔�o�b�t�@�X�V�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}