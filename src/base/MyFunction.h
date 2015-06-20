#ifndef _MY_FUNCTION_H_
#define _MY_FUNCTION_H_

#include <d3d9.h>
#include"define.h"
#include<DirectXMath.h>

using namespace DirectX;

float frand(unsigned v);

void M_VectorSpring(vec* pV, vec* pMark,
					vec* pNow, FLOAT K, FLOAT friction = 0.02f);
//自作関数　D3DXVector3用バネ計算関数
//pV:錘の移動スピード pMark:バネの目標地点・静止位置 pNow:錘の現在地 K:バネ係数 friction:摩擦係数

//D3DX系変数の便利関数
void M_MatrixOffset(mat4 *pOut, vec *pOffset);
void M_MatToVec3X(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Y(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Z(vec3 *pOut, mat4 *pMat);
void M_MatToVec3Pos(vec3 *pOut, mat4 *pMat);
void M_Vec3Copy(vec3 *pOut, vec3 *pOrg);

//PosIdx関係の処理関数
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