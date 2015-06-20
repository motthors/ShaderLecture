#include"MyFunction.h"

float frand(unsigned v)
{
	unsigned res = (v>>9)|0x3f800000;
	return (*(float*)&res)-1.f;
}

void M_VectorSpring(vec* pV, vec* pMark,
					vec* pNow, FLOAT K, FLOAT frict)
{
	vec SpringLen;
	SpringLen = XMVectorSubtract(*pMark, *pNow);
	SpringLen = XMVectorScale(SpringLen, K);
	SpringLen = XMVectorAdd(*pV, SpringLen);
	*pV = XMVectorScale(SpringLen, frict);
	*pNow = XMVectorAdd(*pNow, *pV);
}

void M_MatrixOffset(mat4 *pOut,vec3 *pOffset)
{
	pOut->_41 = pOffset->x;
	pOut->_42 = pOffset->y;
	pOut->_43 = pOffset->z;
}

void M_MatToVec3X(vec3 *pOut, mat4 *pMat)
{
	pOut->x = pMat->_11;
	pOut->y = pMat->_12;
	pOut->z = pMat->_13;
	//*pOut = XMfloat3Normalize(*pOut);
}
void M_MatToVec3Y(vec3 *pOut, mat4 *pMat)
{
	pOut->x = pMat->_21;
	pOut->y = pMat->_22;
	pOut->z = pMat->_23;
	//*pOut = XMVector3Normalize(*pOut);
}
void M_MatToVec3Z(vec3 *pOut, mat4 *pMat)
{
	pOut->x = pMat->_31;
	pOut->y = pMat->_32;
	pOut->z = pMat->_33;
	//*pOut = XMVector3Normalize(*pOut);
}
void M_MatToVec3Pos(vec3 *pOut, mat4 *pMat)
{
	pOut->x = pMat->_41;
	pOut->y = pMat->_42;
	pOut->z = pMat->_43;
	//*pOut = XMVector3Normalize(*pOut);
}

void M_D3DXVec3Copy(vec3 *pOut, vec3 *pOrg)
{
	//pOut->x = pOrg->x;
	//pOut->y = pOrg->y;
	//pOut->z = pOrg->z;
	memcpy_s(pOut, sizeof(vec3), pOrg, sizeof(vec3));
}


//void PosIdx2Pos(vec *pOut, vec *pSrc, POSINDEX *pPosidx)
////dest = src + dist*len
//{
//	XMFLOAT3 v3;
//	XMFLOAT3 d3;
//	XMStoreFloat3(&v3, *pSrc);
//	XMStoreFloat3(&d3, *pOut);
//	d3.x = pPosidx->x * POSIDX_LENGTH + v3.x;
//	d3.y = pPosidx->y * POSIDX_LENGTH + v3.y;
//	d3.z = pPosidx->z * POSIDX_LENGTH + v3.z;
//	*pOut = XMLoadFloat3(&d3);
//}

//void Pos2PosIdx(vec3 *pOut, POSINDEX *pInOutPosIdx, vec3 *pSrc)
//pOut = mod(pSrc,PI_Len);
//pInOutPI += (pSrc - pOut)/PI_Len;
//{
	//float t;
	//t = Src->x;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; destIdx->x += 1; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; destIdx->x -= 1; }
	//Dest->x = t;
	//t = Src->y;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; destIdx->y += 1; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; destIdx->y -= 1; } 
	//Dest->y = t;
	//t = Src->z;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; destIdx->z += 1; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; destIdx->z -= 1; } 
	//Dest->z = t;


	//XMVECTOR mod = XMVectorReplicate(POSIDX_LENGTH);
	//XMVECTOR idx;
	//*pOut = XMVectorMod(*pSrc, mod);
	//idx = XMVectorSubtract(*pSrc, *pOut);
	//idx = XMVectorMultiply(idx, XMVectorReplicate(1.f / POSIDX_LENGTH));
	//XMFLOAT3 f3;
	//XMStoreFloat3(&f3,idx);
	//pInOutPosIdx->x += (ONEOF_PI)f3.x;
	//pInOutPosIdx->y += (ONEOF_PI)f3.y;
	//pInOutPosIdx->z += (ONEOF_PI)f3.z;
//}

void Pos2PosIdx(vec *pOut, vec *pSrc)
{
	//float t;
	//t = Src->x;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; }
	//Dest->x = t;
	//t = Src->y;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; } 
	//Dest->y = t;
	//t = Src->z;
	//while( t >  POSIDX_LENGTH ){ t -= POSIDX_LENGTH; }
	//while( t < -POSIDX_LENGTH ){ t += POSIDX_LENGTH; } 
	//Dest->z = t;
	//XMVECTOR mod = XMVectorReplicate(POSIDX_LENGTH);
	//*pOut = XMVectorMod(*pSrc, mod);
}

//void PosIdxAdd(POSINDEX *InOut, POSINDEX *In)
//// InPOut += In
//{
//	InOut->x += In->x;
//	InOut->y += In->y;
//	InOut->z += In->z;
//}
//void PosIdxAdd(POSINDEX *pOut, POSINDEX *In1, POSINDEX *In2)
//// InPOut = In1 + In2
//{
//	pOut->x = In1->x + In2->x;
//	pOut->y = In1->y + In2->y;
//	pOut->z = In1->z + In2->z;
//}
//void PosIdxSub(POSINDEX *InOut, POSINDEX *In)
//// InPOut -= In
//{
//	InOut->x -= In->x;
//	InOut->y -= In->y;
//	InOut->z -= In->z;
//}
//void PosIdxSub(POSINDEX *pOut, POSINDEX *In1, POSINDEX *In2)
//// InPOut = In1 - In2
//{
//	pOut->x = In1->x - In2->x;
//	pOut->y = In1->y - In2->y;
//	pOut->z = In1->z - In2->z;
//}


//Pos2PosIdxがコレの代わりを行える
//BYTE CheckTransPosIndex(D3DXVECTOR3* pPos, POSINDEX* pIdx)
////pPosがハコの外に出たら次のハコに移す
//{
//	BYTE ret = 0;
//	ret |= Transfunction(&pPos->x, &pIdx->x, 1);
//	ret |= Transfunction(&pPos->y, &pIdx->y, 2) << 2;
//	ret |= Transfunction(&pPos->z, &pIdx->z, 3) << 4;
//	return ret;
//}
//
//inline int Transfunction(float* pPos, ONEOF_PI* pIdx, int flag)
//{
//	if (*pPos >= POSIDX_LENGTH)
//	{
//		*pPos -= POSIDX_LENGTH;
//		*pIdx += 1;
//		return 0x1;
//	}
//	else if (*pPos <= -POSIDX_LENGTH)
//	{
//		*pPos += POSIDX_LENGTH;
//		*pIdx -= 1;
//		return 0x2;
//	}
//	return 0;
//}
//
//void TransPosIndexByFlag(D3DXVECTOR3* pPos, POSINDEX* pIdx, BYTE flag)
////CheckTransPosIndexの戻り値を使って処理
//{
//	TransFunctionByFlag(&pPos->x, &pIdx->x, flag & 0x3);
//	flag >>= 2;
//	TransFunctionByFlag(&pPos->x, &pIdx->x, flag & 0x3);
//	flag >>= 2;
//	TransFunctionByFlag(&pPos->x, &pIdx->x, flag & 0x3);
//}
//
//void TransFunctionByFlag(float* pPos, ONEOF_PI* pIdx, int flag)
//{
//	if (flag == 0x1)
//	{
//		*pPos -= POSIDX_LENGTH;
//		*pIdx += 1;
//	}
//	else if (flag == 0x2)
//	{
//		*pPos += POSIDX_LENGTH;
//		*pIdx -= 1;
//	}
//}
//
//void TransPosIndexByFlag(D3DXVECTOR3* pPos, BYTE flag)
////CheckTransPosIndexの戻り値を使って処理
//{
//	TransFunctionByFlag(&pPos->x, flag & 0x3);
//	flag >>= 2;
//	TransFunctionByFlag(&pPos->x, flag & 0x3);
//	flag >>= 2;
//	TransFunctionByFlag(&pPos->x, flag & 0x3);
//}
//
//void TransFunctionByFlag(float* pPos, int flag)
//{
//	if (flag == 0x1)
//	{
//		*pPos -= POSIDX_LENGTH;
//	}
//	else if (flag == 0x2)
//	{
//		*pPos += POSIDX_LENGTH;
//	}
//}