#include"ShaderLectureMAIN.h"


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

/*/////////////////////////////////////////////////////////////////////////////////////

�EInit�֐�
�v���O�����N������1�񂾂��Ă΂�܂��B
�����Ń��b�V����X�e�[�g�̐ݒ��V�F�[�_�̃��[�h�Ȃǂ��s���܂��B

//////////////////////////////////////////////////////////////////////////////////////*/

typedef struct{
	float x, y, z;		// ���W
	float u, v;			// UV���W
	// DWROD color;		// �F���
} VERTEX;


void 
ShaderLectureMAIN::Init()
{
	///////////////////////////////////////////////////////////
	// ���C���ł����郁�b�V���̍\��   
	// ���_�ƁA�ǂ̒��_���ǂ̏��ԂŌ��Ԃ���\���C���f�b�N�X������܂�

	// ���_���W
	VERTEX vtx[4];
	vtx[0].x = -1.0f;	vtx[0].y = 1.0f;	vtx[0].z = 0.0f;
	vtx[1].x = 0.5f;	vtx[1].y = 0.5f;	vtx[1].z = 0.0f;
	vtx[2].x = -0.5f;	vtx[2].y = -0.5f;	vtx[2].z = 0.0f;
	vtx[3].x = 1.0f;	vtx[3].y = -1.0f;	vtx[3].z = 0.0f;
	// �e�N�Z��
	vtx[0].u = 0.0f + 0.5f / WINDOW_SIZE_W; vtx[0].v = 0.0f + 0.5f / WINDOW_SIZE_H;
	vtx[1].u = 1.0f + 0.5f / WINDOW_SIZE_W; vtx[1].v = 0.0f + 0.5f / WINDOW_SIZE_H;
	vtx[2].u = 0.0f + 0.5f / WINDOW_SIZE_W; vtx[2].v = 1.0f + 0.5f / WINDOW_SIZE_H;
	vtx[3].u = 1.0f + 0.5f / WINDOW_SIZE_W; vtx[3].v = 1.0f + 0.5f / WINDOW_SIZE_H;
	// ���_�o�b�t�@���쐬
	m_pDxBase->CreateVertexBuffer(&m_pVertexBuffer, vtx, sizeof(vtx), 0);

	// �C���f�b�N�X�o�b�t�@�쐬
	USHORT Index[] = { 0, 1, 2, 3 };
	m_pDxBase->CreateIndexBuffer(&m_pIndexBuffer, Index, sizeof(Index), 0);



	///////////////////////////////////////////////////////////
	// �萔�o�b�t�@�̍쐬
	// �S�ẴV�F�[�_�X���b�h�ŋ��ʂ̕ϐ��ɓo�^���\�ȃo�b�t�@���쐬���܂��B
	// C,C++�ł�static�̂悤�Ȃ��̂ŁADirectX9�ł��g�������ł��B

	m_pDxBase->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(m2DDWConstVS), D3D11_CPU_ACCESS_WRITE);
	m_pDxBase->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(m2DDWConstPS), D3D11_CPU_ACCESS_WRITE);
	
	

	///////////////////////////////////////////////////////////
	// ���̓��C�A�E�g�̐ݒ�
	// ���_�V�F�[�_�̓��͂��V�F�[�_�ɋ����邽�߂̐ݒ����ݒ肵�܂��B
	// �f�t�H���g�ł͒��_���W3�����ƃe�N�X�`�����W2�����ł��B

	D3D11_INPUT_ELEMENT_DESC m_pLayoutDesc[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	///////////////////////////////////////////////////////////
	// �V�F�[�_�̃��[�h
	// VisualStudio��hlsl�̃R���p�C�����\�ł��B
	// �v���W�F�N�g�Ɋ܂߂ăV�F�[�_�̎�ނ�ݒ肵�Ă����΁A�r���h�̂��тɃR���p�C�����Ă���܂��B
	//�@�R���p�C�����ꂽ�o�C�i���t�@�C��(.cso)�����̂܂ܓǂݍ����GPU�ւ̓o�^���\�ł��B

	//�V�F�[�_�̍쐬�@���_�V�F�[�_�͓��̓��C�A�E�g�ƃZ�b�g�œo�^���Ȃ��Ƃ����܂���B
	CreateVertexShader(&m_pVertexShader, _T("src/shader/VS_normal.cso"),
		m_pLayoutDesc, sizeof(m_pLayoutDesc) / sizeof(m_pLayoutDesc[0]), &m_pLayout);
	CreatePixelShader(&m_pPixelShader, _T("src/shader/PS_RefTex.cso"));


	///////////////////////////////////////////////////////////
	// �r���[�|�[�g�̐ݒ�
	// �쐬�����E�B���h�E�̃T�C�Y(+��)���V�F�[�_�ɋ����Ă����܂��B

	D3D11_VIEWPORT	vp;
	vp.Width = static_cast<float>(WINDOW_SIZE_W);
	vp.Height = static_cast<float>(WINDOW_SIZE_H);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1, &vp);




	/////////////////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------------
	// �X�e�[�g�쐬
	// �e�V�F�[�_�X�e�[�W�ɑ΂��Ăǂ̂悤�ȓ���������΂�������ݒ肵�܂��B
	//------------------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////////////
	

	///////////////////////////////////////////////////////////
	// �u�����h�X�e�[�g
	// �V�����I�u�W�F�N�g�Ȃǂ̊G�Ƃ���܂łɕ`�悵�Ă������G���ǂ̂悤�ɍ������邩��ݒ肵�܂��B
	// �������̃A���t�@��������点�鎞�Ȃǂ̉��Z�����A���Ɍ��Z���Z�Ȃǂ��\�ł��B

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

	HRESULT hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pTextureBlendState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateBlendState"));
		ErrM.SetErrorText(_T("�u�����h�X�e�[�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	///////////////////////////////////////////////////////////
	// ���X�^���C�U�X�e�[�g
	// ���_�V�F�[�_�̃f�[�^�ł���_�Ɛ�����A
	// �s�N�Z���V�F�[�_�̃f�[�^�ł���s�N�Z���̐F��h�邩�ǂ����̔�����s���X�e�[�g�̐ݒ�ł��B
	// �ʏ�̖ʕ`��̂ق��Ƀ��C���[�t���[���̕`��ݒ��O�p�`�|���S���̗��\����ƕ`��ۂ̔���������ł��B

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.DepthClipEnable = TRUE;
	hr = m_pDevice->CreateRasterizerState(&rsDesc, &m_pRasterizerState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateRasterizerState"));
		ErrM.SetErrorText(_T("���X�^���C�U�X�e�[�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}


	///////////////////////////////////////////////////////////
	// �f�v�X�X�e���V���X�e�[�g
	// �[�x�l�̏�񂩂�`�悷�邩���Ȃ����̔����A�}�X�N��p�����`�搧����s���X�e�[�g�̐ݒ�ł��B

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;

	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_DepthState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateDepthStencilState"));
		ErrM.SetErrorText(_T("�f�v�X�X�e���V���X�e�[�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	///////////////////////////////////////////////////////////
	// �T���v���X�e�[�g
	// �s�N�Z���V�F�[�_�ȂǂŃe�N�X�`�����Q�Ƃ���Ƃ��̕��@��ݒ肵�܂��B
	// �e�N�X�`���͈̔͊O���w�肳�ꂽ�Ƃ��ɂǂ����邩�@��A
	// �g��k���������Ƃ��ɂǂ̂悤�ȏ��������邩�@�Ȃǂ̐ݒ�����܂��B
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
		ErrM.SetErrorText(_T("�T���v���X�e�[�g�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	
	// �����_�����O�^�[�Q�b�g��ݒ肷��
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// �萔�o�b�t�@�̐��l�ݒ�
	XMStoreFloat4x4(&m_ConstVS.World, XMMatrixTranspose(XMMatrixIdentity()));
}

/*/////////////////////////////////////////////////////////////////////////////////////

�EMove�֐�
������Update�֐��ł�
�`�惋�[�v���̍ŏ��̂ق��ŌĂ΂�܂��B
���_�̐����I�u�W�F�N�g�̈ړ������Ȃǂ�����Ƃ������Ǝv���܂��B

//////////////////////////////////////////////////////////////////////////////////////*/

void
ShaderLectureMAIN::Move()
{

}


/*/////////////////////////////////////////////////////////////////////////////////////

�ESetDataToShader�֐�
��q��Draw�֐��̒��O�ŌĂ΂�܂��B
Draw�֐��ƈꏏ�ɂ��Ă������ł����A�����Ȃ�̂ŃX�e�[�g�̓o�^�Ȃǂ𕪗����܂����B�i�Ƃ肠�����j

//////////////////////////////////////////////////////////////////////////////////////*/

void
ShaderLectureMAIN::SetDataToShader()
{
	// VS�֒萔�o�b�t�@�ݒ� �s���]�u���ēo�^�킷�ꂸ��
	mat V = XMLoadFloat4x4(m_pCamera->GetView());
	mat P = XMLoadFloat4x4(m_pCamera->GetPers());
	V = XMMatrixTranspose(V);
	P = XMMatrixTranspose(P);
	XMStoreFloat4x4(&m_ConstVS.View, V);
	XMStoreFloat4x4(&m_ConstVS.Proj, P);
	m_pDxBase->SetConstBuffer(m_pConstantBafferVS, reinterpret_cast<void*>(&m_ConstVS), sizeof(m_ConstVS));
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
	// PS�֒萔�o�b�t�@�ݒ�
	m_pDxBase->SetConstBuffer(m_pConstantBafferPS, reinterpret_cast<void*>(&m_ConstPS), sizeof(m_ConstPS));
	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);

	//�V�F�[�_���Z�b�g
	m_pContext->IASetInputLayout(m_pLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Set State
	m_pContext->OMSetBlendState(m_pTextureBlendState, Colors::Black, 0xffffffff);
	m_pContext->RSSetState(m_pRasterizerState);
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);
	m_pContext->OMSetDepthStencilState(m_DepthState, 0);
}



/*/////////////////////////////////////////////////////////////////////////////////////

�EDraw�֐�
�I�u�W�F�N�g�̕`����s���܂��B
������K�v�͍��̂Ƃ��떳���������E�E�E�H

//////////////////////////////////////////////////////////////////////////////////////*/

void
ShaderLectureMAIN::Draw()
{
	/////Draw
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_pContext->DrawIndexed(4, 0, 0);
}
