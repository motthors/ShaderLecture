
//////////////////////////////////////////////////////////////
// ピクセルシェーダテスト

Texture2D  Tex  : register(t0);
SamplerState Sampler  : register(s0);

struct PS_IN
{
	float4 pos	: SV_POSITION;
	float2 uv   : TEXCOORD0;
};

struct PS_OUT
{
	float4 MainCol	: SV_Target0;
};


PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.MainCol = float4(In.uv.x, 0, In.uv.y, 1);//Tex.Sample(Sampler, In.uv);

	return Out;
}

