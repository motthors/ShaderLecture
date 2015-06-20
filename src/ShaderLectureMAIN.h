#pragma once 

#include"ShaderLectureBase.h"

class ShaderLectureMAIN : public ShaderLectureBase{

	// バッファ
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	// シェーダ
	ID3D11VertexShader*	m_pVertexShader = nullptr;
	ID3D11PixelShader*	m_pPixelShader = nullptr;
	ID3D11InputLayout*	m_pLayout = nullptr;

	// 定数バッファ構造体宣言
	struct m2DDWConstVS{
		vec4 Width_Shift;
		float FakeDepth;
		vec3 __Blank;
	} m_ConstVS;
	struct m2DDWConstPS{
		float alpha;
		float XperY;
		vec2 __Blank;
		vec4 Width_Shift;
	} m_ConstPS;
	// 定数バッファ
	ID3D11Buffer*	m_pConstantBafferVS = nullptr;
	ID3D11Buffer*	m_pConstantBafferPS = nullptr;

	// 共通ステート DXTK
	ID3D11RasterizerState*		m_pRasterizerState = nullptr;
	ID3D11BlendState*			m_pTextureBlendState = nullptr;
	ID3D11DepthStencilState*	m_pCommonDepthStencilState = nullptr;
	ID3D11SamplerState*			m_pSamplerState = nullptr;
	ID3D11DepthStencilState*	m_DepthState = nullptr;


public:
	ShaderLectureMAIN();
	virtual ~ShaderLectureMAIN();

	void Init();
	void Move();
	void SetDataToShader();
	void Draw();

};