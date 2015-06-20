// �T���v���`��

//! �R���X�^���g�o�b�t�@
/*!
�R���X�^���g�o�b�t�@���g�p���邱�Ƃł������̃R���X�^���g���W�X�^��1�܂Ƃ߂ɂ��邱�Ƃ��ł���
���W�X�^�̕ω��̃^�C�~���O�ɂ���ĕ����Ă��������悢
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

//! ���_����
/*!
��{��D3D9�ƕς��Ȃ�
�������A�������̃Z�}���e�B�N�X���ς���Ă���
�V�X�e�����ɓn���Z�}���e�B�N�X��SV_�̐ړ��������Ă���
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

//! ���_�V�F�[�_
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

//! �s�N�Z���V�F�[�_
float4 RenderPS( OutputVS inPixel ) : SV_TARGET
{
	return inPixel.color;
}
