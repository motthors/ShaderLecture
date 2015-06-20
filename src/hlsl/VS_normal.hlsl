
//////////////////////////////////////////////////////////////
// äOïîÇ©ÇÁê›íËÇ∑ÇÈïœêî

cbuffer cbSceneParam : register(b0)
{
	matrix matWorld : packoffset(c0);
	matrix matView : packoffset(c4);
	matrix matProj : packoffset(c8);
};

//////////////////////////////////////////////////////////////
// vs1
struct VS_IN
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos.w = 1.0;
	//Out.Pos = mul(In.Pos, matWorld);
	//Out.Pos = mul(Out.Pos, matView);
	//Out.Pos = mul(Out.Pos, matProj);
	Out.Pos = In.Pos;
	Out.uv = In.uv;

	return Out;
}
