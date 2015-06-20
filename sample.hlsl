// サンプル描画

//! コンスタントバッファ
/*!
コンスタントバッファを使用することでいくつかのコンスタントレジスタを1まとめにすることができる
レジスタの変化のタイミングによって分けておく方がよい
*/
cbuffer cbSceneParam : register( b0 )
{
	matrix	mtxView			: packoffset( c0 );
	matrix	mtxProj			: packoffset( c4 );
};

cbuffer cbMeshParam : register( b1 )
{
	matrix	mtxWorld		: packoffset( c0 );
};

//! 頂点属性
/*!
基本はD3D9と変わらない
ただし、いくつかのセマンティクスが変わっている
システム側に渡すセマンティクスはSV_の接頭辞がついている
*/
struct InputVS
{
	float4	pos			: IN_POSITION;
	float4	color		: IN_COLOR0;
};
struct OutputVS
{
	float4	pos			: SV_POSITION;
	float4	color		: COLOR0;
};

//! 頂点シェーダ
OutputVS RenderVS( InputVS inVert )
{
	OutputVS	outVert;

	matrix	mtxVP = mul( mtxView, mtxProj );
	matrix	mtxWVP = mul( mtxWorld, mtxVP );
	outVert.pos = mul( inVert.pos, mtxWVP );

	outVert.color = inVert.color;
//	outVert.color = (float4)0;

	return outVert;
}

//! ピクセルシェーダ
float4 RenderPS( OutputVS inPixel ) : SV_TARGET
{
	return inPixel.color;
}
