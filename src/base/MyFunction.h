#ifndef _MY_FUNCTION_H_
#define _MY_FUNCTION_H_

#include <d3d9.h>
#include"define.h"
#include<DirectXMath.h>

using namespace DirectX;

float frand(unsigned v);

void M_VectorSpring(vec* pV, vec* pMark,
					vec* pNow, FLOAT K, FLOAT friction = 0.02f);
//����֐��@D3DXVector3�p�o�l�v�Z�֐�
//pV:���̈ړ��X�s�[�h pMark:�o�l�̖ڕW�n�_�E�Î~�ʒu pNow:���̌��ݒn K:�o�l�W�� friction:���C�W��

//D3DX�n�ϐ��֗̕��֐�
void M_MatrixOffset(mat4 *pOut, vec *pOffset);
void M_MatToVec3X(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Y(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Z(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Pos(vec3 *pOut, mat4 *pMat);
void M_Vec3Copy(vec3 *pOut, vec3 *pOrg);

//PosIdx�֌W�̏����֐�
//void PosIdx2Pos(vec3 *pOut, vec3 *pSrc, POSINDEX *pPosidx);
//void Pos2PosIdx(vec3 *pOut, POSINDEX *pInOutPosIdx, vec3 *pSrc);
//void Pos2PosIdx(vec3 *pOut, vec3 *pSrc);

//void PosIdxAdd(POSINDEX *InOut, POSINDEX *In);
//void PosIdxAdd(POSINDEX *pOut, POSINDEX *In1, POSINDEX *In2);
//void PosIdxSub(POSINDEX *InOut, POSINDEX *In);
//void PosIdxSub(POSINDEX *pOut, POSINDEX *In1, POSINDEX *In2);

//BYTE CheckTransPosIndex(vec3* pPos, POSINDEX* pIdx);
//inline int Transfunction(float* pPos, ONEOF_PI* pIdx, int flag);
//void TransPosIndexByFlag(vec3* pPos, POSINDEX* pIdx, BYTE flag);
//void TransPosIndexByFlag(vec3* pPos, BYTE flag);
//inline void TransFunctionByFlag(float* pPos, ONEOF_PI* pIdx, int flag);
//inline void TransFunctionByFlag(float* pPos, int flag);


#endif