#include"Camera.h"

Camera::Camera()
{
	Reset();

	FollowObjectFunc[0] = &Camera::FollowObject_inside;
	FollowObjectFunc[1] = &Camera::FollowObject_outside;

	SetCameraRotaFunc[0] = &Camera::SetCameraRotaX_inside;
	SetCameraRotaFunc[1] = &Camera::SetCameraRotaX_outside;

	SetCameraRotaFunc[2] = &Camera::SetCameraRotaY_inside;
	SetCameraRotaFunc[3] = &Camera::SetCameraRotaY_outside; 

	SetPers( XMConvertToRadians(60), 0.1f, 10000000.0f);
	//SetCameraPos(&XMVectorSet(-1250.f, -33.f, 2300.f, 0.0f));
	//XMFLOAT3 t_pos;
	//Pos2PosIdx(&t_pos, &posidx, &t_pos);
	//SetCameraDir(&XMVectorSet(-100000.f, -1000.f, -20000.f, 0.0f));
}

void Camera::Reset()
{
	XMStoreFloat4x4(&view, XMMatrixIdentity());
	XMStoreFloat4x4(&pers, XMMatrixIdentity());

	//posidx.x = posidx.y = posidx.z = 0;

	len = 0.0f;
	ZeroMemory(&pos, sizeof(vec3));
	ZeroMemory(&CamRot, sizeof(vec3));
	dir = vec3(0.0f, 0.0f, 1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	memcpy_s(&v_pos, sizeof(vec3), &pos, sizeof(vec3));
	memcpy_s(&v_dir, sizeof(vec3), &dir, sizeof(vec3));
	memcpy_s(&v_up, sizeof(vec3), &up, sizeof(vec3));

	cameraX = 0.0f;
	cameraY = 0.0f;

	ViewportWidth = 100.0f;
	ViewportHeight = 100.0f;

	SetPers(XMConvertToRadians(60), 0.1f, 10000000.0f);
}

void Camera::SetTransform(DXDEVICE* pd3ddev)
{
	//pd3ddev->SetViewport(&ViewData);
}

void Camera::SetViewPort(int playerNum, int CameraNum, float WindW, float WindH)
{
	DWORD x, y;
	// 左上設定
	x = CameraNum%2 == 0	? 0 : (DWORD)(WindW/2);
	y = CameraNum < 2		? 0 : (DWORD)(WindH/2);
	// 画面の幅設定
	ViewportWidth  = playerNum > 1	? WindW/2 : WindW;
	ViewportHeight = playerNum > 2	? WindH/2 : WindH;

	ViewData.X		= x;
	ViewData.Y		= y;
	ViewData.Width	= (DWORD)ViewportWidth;	//レンダーターゲットのサイズ
	ViewData.Height	= (DWORD)ViewportHeight;
	ViewData.MinZ	= 0.0f;
	ViewData.MaxZ	= 1.0f;
}


void Camera::SetView(vec *POS, vec *DIR, vec *UP)
{
	vec AT;
	AT = XMVectorAdd(*POS, *DIR);
	XMStoreFloat4x4(&view, XMMatrixLookAtLH(*POS, AT, *UP));
}
void Camera::SetView(vec3 *POS, vec3 *DIR, vec3 *UP)
{
	vec AT;
	vec t_POS = XMLoadFloat3(POS);
	vec t_DIR = XMLoadFloat3(DIR);
	vec t_UP = XMLoadFloat3(UP);
	AT = XMVectorAdd(t_POS, t_DIR);
	XMStoreFloat4x4(&view, XMMatrixLookAtLH(t_POS, AT, t_UP));
}

void Camera::SetPers(float fovYRad,float znear,float zfar)
{
	mat t_p = XMMatrixPerspectiveFovLH(fovYRad, ViewportWidth/ViewportHeight, znear, zfar);
	XMStoreFloat4x4(&pers, t_p);
}

void Camera::SetViewLerp(vec *POS, vec *DIR, vec *UP,
							FLOAT LEN, FLOAT posk, FLOAT dirk, FLOAT upk)
{
	vec t_pos = XMLoadFloat3(&pos);
	vec t_dir = XMLoadFloat3(&dir);
	vec t_up = XMLoadFloat3(&up);
	//バネ機構のような線形補間
	len = len + (LEN-len)*0.9f;
	t_pos = XMVectorLerp(t_pos, *POS, posk);
	t_dir = XMVectorLerp(t_dir, *DIR, dirk);
	t_up	= XMVectorLerp(t_up, *UP, upk);
	XMStoreFloat3(&pos, t_pos);
	XMStoreFloat3(&dir, t_dir);
	XMStoreFloat3(&up, t_up);
}

void Camera::SetViewSpring(vec *POS, vec *DIR, vec *UP,
							FLOAT LEN, FLOAT posk, FLOAT dirk, FLOAT upk)
{
	vec t_pos = XMLoadFloat3(&pos);
	vec t_dir = XMLoadFloat3(&dir);
	vec t_up = XMLoadFloat3(&up);
	vec t_v_pos = XMLoadFloat3(&v_pos);
	vec t_v_dir = XMLoadFloat3(&v_dir);
	vec t_v_up = XMLoadFloat3(&v_up);
	len = len + (LEN-len)*0.1f;
	M_VectorSpring(&t_v_pos, POS, &t_pos, posk, 0.0194f);
	M_VectorSpring(&t_v_dir, DIR, &t_dir, dirk, 0.204f);
	M_VectorSpring(&t_v_up, UP, &t_up, upk, 0.124f);
	XMStoreFloat3(&pos, t_pos);
	XMStoreFloat3(&dir, t_dir);
	XMStoreFloat3(&up, t_up);
	XMStoreFloat3(&v_pos, t_v_pos);
	XMStoreFloat3(&v_dir, t_v_dir);
	XMStoreFloat3(&v_up, t_v_up);
}

void Camera::FreeLookCameraCalc(vec3* pOut, vec* OrgDir, vec* x, vec* y)

{
	vec t_Out = XMLoadFloat3(pOut);

	t_Out = XMVector3Normalize(t_Out);
	vec l_dir = *OrgDir;
	mat camY,camX;
	//CameraXYをもとに回転行列作成
	camX = XMMatrixRotationAxis(*x, cameraX);
	camY = XMMatrixRotationAxis(*y, cameraY);
	// Y軸回転後にX軸回転
	l_dir = XMVector3TransformNormal(l_dir, camY);
	l_dir = XMVector3TransformNormal(l_dir, camX);

	//フリールックベクトルのバネ
	t_Out = XMVectorLerp(t_Out, l_dir, 0.2f);

	XMStoreFloat3(pOut, t_Out);
}


///////////////////////////////////////////////////////////////////////
///////////////////////カメラコントローラー////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void Camera::FollowObject(vec *p, mat4 *rota, int flag)
{
	vec3 d3,u3,y3;
	M_MatToVec3Z(&d3,rota);
	M_MatToVec3Y(&u3,rota);
	M_MatToVec3X(&y3,rota);
	vec d = XMLoadFloat3(&d3);
	vec u = XMLoadFloat3(&u3);
	vec y = XMLoadFloat3(&y3);
	
	//(this->*FollowObjectFunc[flag])(p,&d,&u);
	//SetViewLerp(p, &d, &u, (FLOAT)CameraLen, 0.97f, 0.94f,0.3f);

	/////視点変更のベクトル計算
	FreeLookCameraCalc(&CamRot, &d, &u, &y);

	//カメラ位置ベクトル計算
	vec temp;
	vec t_CamRot = XMLoadFloat3(&CamRot);
	vec t_up = XMLoadFloat3(&up);
	//D3DXVec3Add( &l_p, &pos, D3DXVec3Scale(&d, &d, 0.2f*len) );
	y = XMVectorSubtract(t_CamRot, XMVectorScale(t_up, 0.05f*len));
	XMStoreFloat3(&CamRot, t_CamRot);
	XMStoreFloat3(&up, t_up);

	y = XMVectorScale(y, len);
	y = XMVectorSubtract(*p, y);
	//D3DXVec3Subtract(&y, &pos, &y);
	(this->*FollowObjectFunc[flag])(&y,&d,&u);
	vec t_pos = XMLoadFloat3(&pos);
	SetView(&t_pos, &t_CamRot, &t_up);
	XMStoreFloat3(&pos, t_pos);
}
void Camera::FollowObject_inside(vec *p,vec *d,vec *u)
{
	//SetViewLerp(p, d, u, 0.0f, 0.99f, 0.94f,0.3f);
	SetViewSpring(p, d, u, 0.0f, 50.f, 0.6f,0.9f);
}
void Camera::FollowObject_outside(vec *p,vec *d,vec *u)
{
	//SetViewLerp(p, d, u, 4.0f, 0.9f, 0.07f,0.1f);
	SetViewSpring(p, d, u, 4.0f, 50.9f, 0.07f,0.3f);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//void Camera::LookObject(_BaseObject *pObj){
//	D3DXVECTOR3 *p	= pObj->GetPos();
//	D3DXVECTOR3 d;
//	D3DXVec3Subtract(&d,p,&pos);
//	SetViewSpring(&pos, &d, &up, 0.0f, 1.0f, 1.0f, 1.0f);
//	SetView(&pos, &dir, &up);
//}
void Camera::LookObject(vec *p, float posk, float dirk, float atk){

	vec d;
	//d = XMVectorSubtract(*p,pos);
	SetViewSpring(p, &d, &XMVectorSet(0.f,1.f,0.f,0.0f), 0.0f, posk, dirk, atk);
	//Pos2PosIdx(&pos, &posidx, &pos);
	SetView(&pos, &dir, &up);
}

void Camera::Goal(vec *TimeBoardPos)
{
	//慣性＋ブレーキ
	//pos = XMVectorAdd(pos,v_pos);
	//v_pos = XMVectorScale(v_pos,0.996f);

	//vec d;
	////d = XMVectorSubtract(*TimeBoardPos,pos);

	//dirとupだけバネ
	//M_VectorSpring(&v_dir, &d,						&dir, 0.00009f, 0.154f);
	//M_VectorSpring(&v_up, &XMVectorSet(0.f, 1.f, 0.f, 0.f), &up, 0.05f, 0.124f);
	//Pos2PosIdx(&pos, &posidx, &pos);
	SetView(&pos, &dir, &up);
}
void Camera::FreeLook(vec *p,vec *d,vec *u,
					float posk, float dirk, float atk){
	//SetViewSpring(p, d, u, 0.0f, posk, dirk, atk);
	//Pos2PosIdx(&pos, &posidx, &pos);
	SetView(&pos, &dir, &up);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void Camera::CourseEdit(vec *p, float CamLen){
	/////視点変更のベクトル計算
	FreeLookCameraCalc(&CamRot,
		&XMVectorSet(0, 0, 1, 0),
		&XMVectorSet(0, 1, 0, 0),
		&XMVectorSet(1, 0, 0, 0)
	);

	//位置ベクトル計算
	//vec t;
	//t = XMVectorScale(CamRot, CamLen);
	//t = XMVectorSubtract(pos, t);

	SetViewSpring(p, 
		&XMVectorSet(1, 0, 0, 0),
		&XMVectorSet(0, 1, 0, 0),
		CamLen, 
		20.3f
	);
	//CheckTransPIOwnCamera();
	//SetView(&t, &CamRot, &up);
}
///////////////////////////////////////////////////////////////////////
void Camera::CourseEdit2(vec *p, mat4 *rota, float CamLen){
	vec d,u;
	//M_MatToVec3Z(&d,rota);
	//M_MatToVec3Y(&u,rota);
	//位置ベクトル計算
	vec t;
	//t = XMVectorScale(d, CamLen);
	//XMVectorSubtract(*p, t);

	//SetViewSpring( &t, &d, &u, CamLen, 20.3f, 0.1f, 1.1f);
	//CheckTransPIOwnCamera();
	//SetView(&pos, &d, &up);
}
///////////////////////////////////////////////////////////////////////
void Camera::Menu(vec *p,vec *at,vec *u,
					float posk, float dirk, float atk)
{
	vec d;
	d = XMVectorSubtract(*at, *p);
	SetViewSpring(p, &d, u, 0.0f, posk, dirk, atk);
	//Pos2PosIdx(&pos, &posidx, &pos);
	//SetView(&pos, &dir, &up);
}
//void Camera::Menu(vec *p,POSINDEX *idx,vec *at,vec *u,
//					float posk, float dirk, float atk)
//{
//	vec d,transpos;
//	d = XMVectorSubtract(*at, *p);
//	POSINDEX dist = *idx;
//	dist.x -= posidx.x;
//	dist.y -= posidx.y;
//	dist.z -= posidx.z;
//	//PosIdx2Pos(&transpos, p, &dist);
//	SetViewSpring(p, &d, u, 0.0f, posk, dirk, atk);
//	//Pos2PosIdx(&pos, &posidx, &pos);
//	SetView(&pos, &dir, &up);
//}
///////////////////////////////////////////////////////////////////////

void Camera::PASCamera(vec BasePoint, vec Direc, vec UP, float Length)
{
	vec x;
	x = XMVector3Cross(Direc, UP);

	/////視点変更のベクトル計算
	FreeLookCameraCalc(&CamRot, &Direc, &UP, &x);

	//カメラ位置ベクトル計算 変数再利用　x -> TransedPosition
	vec temp;
	x = Direc;
	x = XMVectorScale(x, len);
	x = XMVectorSubtract(BasePoint, x);
	SetViewSpring(&x, &Direc, &UP, Length, 50.f, 10.9f, 10.9f);
	SetView(&pos, &CamRot, &up);
}

void Camera::PASCamera(vec3* BasePoint, vec3* Direc, vec3* UP, float Length)
{
	vec x;
	vec t_Direc = XMLoadFloat3(Direc);
	vec t_UP = XMLoadFloat3(UP);
	vec t_BasePoint = XMLoadFloat3(BasePoint);
	x = XMVector3Cross(t_Direc, t_UP);

	/////視点変更のベクトル計算
	FreeLookCameraCalc(&CamRot, &t_Direc, &t_UP, &x);

	//カメラ位置ベクトル計算 変数再利用　x -> TransedPosition
	x = t_Direc;
	x = XMVectorScale(x, len);
	x = XMVectorSubtract(t_BasePoint, x);
	SetViewSpring(&x, &t_Direc, &t_UP, Length, 50.f, 0.6f, 0.9f);
	SetView(&pos, &CamRot, &up);
}

///////////////////////////////////////////////////////////////////////

void Camera::ShaderLecture()
{
	vec x;
	vec t_Direc = XMLoadFloat3(&vec3(0, 0, 1));
	vec t_UP = XMLoadFloat3(&vec3(0, 1, 0));
	x = XMVector3Cross(t_Direc, t_UP);

	/////視点変更のベクトル計算
	FreeLookCameraCalc(&CamRot, &t_Direc, &t_UP, &x);

	//カメラ位置ベクトル計算 変数再利用　x -> TransedPosition
	x = XMLoadFloat3(&CamRot);
	x = XMVectorScale(x, -len);
	SetViewSpring(&x, &t_Direc, &t_UP, /*Length = */5.0f, 50.f, 0.6f, 0.9f);
	SetView(&pos, &CamRot, &up);
}

///////////////////////////////////////////////////////////////////////
void Camera::SetCameraRotaX(FLOAT value, int flag){
	(this->*SetCameraRotaFunc[flag>0])(value);
}
void Camera::SetCameraRotaX_inside(FLOAT xValue){
	cameraX -= xValue;
	if(cameraX > 3.0f)cameraX = 3.0f;
	else if(cameraX < -3.0f)cameraX = -3.0f;
}
void Camera::SetCameraRotaX_outside(FLOAT xValue){
	cameraX += xValue;
	if(cameraX > PI)cameraX -= twoPI;
	else if(cameraX < -PI)cameraX += twoPI;
}

void Camera::SetCameraRotaY(FLOAT value, int flag){
	(this->*SetCameraRotaFunc[(flag>0)+2])(value);
}
void Camera::SetCameraRotaY_inside(FLOAT yValue){
	cameraY += yValue;
	if(cameraY >= hPI-0.1f)cameraY = hPI-0.1f;
	else if(cameraY <= -hPI+0.1f)cameraY = -hPI+0.1f;
}
void Camera::SetCameraRotaY_outside(FLOAT yValue){
	cameraY += yValue;
	if(cameraY >= hPI-0.1f)cameraY = hPI-0.1f;
	else if(cameraY <= -hPI+0.1f)cameraY = -hPI+0.1f;
}
/////////////////////////////////////////////////////////////////////////

void Camera::SetMenuMode()
{
	SetPers(XMConvertToRadians(60), 0.1f, 10000000.0f);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void Camera::SetAspect(float width, float height)
{
	ViewportWidth = width;
	ViewportHeight = height;
}

void Camera::ResetFreeCamera()
{
	cameraX = 0.0f;
	cameraY = 0.0f;
}


void Camera::DebugDraw(DebugCommentManager* pDebugM)
{
	TCHAR txt[700];
	_stprintf_s(txt,699,_T("Camera\n")
						_T("pos:%9.2f/%9.2f/%9.2f\n")
						_T("vpos:%9.2f/%9.2f/%9.2f\n")
						_T("dir:%9.2f/%9.2f/%9.2f\n")
						_T("X:%f/Y:%f\nlen:%f\n"),
		pos.x,pos.y,pos.z,
		v_pos.x,v_pos.y,v_pos.z,
		dir.x,dir.y,dir.z,
		cameraX,cameraY,len
		);
	pDebugM->Set(txt,8);
	pDebugM->End();
}

/////////////////////////////////////////////////////////////////////////


void Camera::ResetCamera()
{
	//posidx.x = posidx.y = posidx.z = 0;

	len = 0.0f;
	ZeroMemory(&CamRot, sizeof(vec3));
	up = vec3(0.0f, 1.0f, 0.0f);
	pos = vec3(0.0f, 0.0f, 0.0f);
	dir = vec3(0.0f, 0.0f, 1.0f);

	v_pos = pos;
	v_dir = dir;
	v_up = up;

	cameraX = 0.0f;
	cameraY = 0.0f;
}