#ifndef _CAMERA_H_
#define _CAMERA_H_
//////////////////////////////////////
/*			カメラクラス
クラスが独自にカメラの情報を持つ
バネカメラ機能持ち
/////////////////////////////////////*/

//class Camera;

#include"IncludeAll.h"
#include"DebugCommentManager.h"
//#include"_BaseObject.h"

//カメラの長さ
#define CAMERA_FOLLOWOBJ_INSIDE		0
#define CAMERA_FOLLOWOBJ_OUTSIDE	1


class Camera{

	mat4		view; 
	mat4		pers;
	//POSINDEX	posidx;

	//バネカメラ用変数群
	FLOAT	len;	//カメラ位置から視点方向へどれだけずらすか
	vec3	pos;	//カメラの位置
	vec3	CamRot; //フリールックカメラの向き
	vec3	dir;	//カメラの視点ベクトル
	vec3	up;		//カメラの上方向
	vec3	v_pos;	//カメラの位置スピード
	vec3	v_dir;	//カメラの視点スピード
	vec3	v_up;	//カメラの上方向スピード

	//フリールックカメラ用変数
	FLOAT	cameraX; //フリールックカメラの向きの回転角度
	FLOAT	cameraY; //フリールックカメラの向きの回転角度

	float ViewportWidth;	// ゲーム画面の横の長さ
	float ViewportHeight;	// ゲーム画面の縦の長さ
	D3DVIEWPORT9 ViewData;	// ビューポート設定用構造体

	void (Camera::*SetCameraRotaFunc[6])(FLOAT);
	void (Camera::*FollowObjectFunc[3])(vec *p, vec *d, vec *u);

public:
	Camera();
	void Reset();

	//生成した各種行列をデバイスにセット
	void SetTransform(DXDEVICE* pd3ddev);
	//総プレイ人数とカメラ番号からAspectRatioとViewportの設定
	void SetViewPort(int playerNum, int CameraNum, float WindW, float WindH);

	//pos,dir,upを設定
	void SetView(vec *pos, vec *dir, vec *up);
	void SetView(vec3 *pos, vec3 *dir, vec3 *up);
	//View行列をセット
	void SetView();
	//射影変換行列をセット
	void SetPers(float fovYRad, float znear, float zfar);

	//バネカメラのつもり　バネカメラ用変数の更新
	void SetViewLerp(vec *POS, vec *DIR, vec *UP,
			FLOAT len, FLOAT posk=0.93f, FLOAT dirk=0.07f, FLOAT upk=0.1f);
	//こっちがほんとのバネカメラ
	void SetViewSpring(vec *POS, vec *DIR, vec *UP,
			FLOAT len,FLOAT posk=20.93f, FLOAT dirk=0.75f, FLOAT upk=1.5f);
	//フリールックカメラの計算（バネ入り）
	void FreeLookCameraCalc(vec3* Out, vec* OrgDir,	vec* x, vec* y);
	
	///////////////////////////////////////////////////////////////////////
	/////////////////////////カメラコントローラー//////////////////////////
	/////////////////////各Move()の一番最後で呼び出す//////////////////////
	//オブジェクトについてゆく
	void FollowObject(vec* pPos, mat4* rota, int CameraLen);
	void FollowObject_inside(vec *p, vec *d, vec *u);
	void FollowObject_outside(vec *p, vec *d, vec *u);

	//オブジェクトをみるだけ　カメラの移動はしない
	//void LookObject(_BaseObject *pObj);
	void LookObject(vec *p, float posk = 50.0f, float dirk = 0.0f, float atk = 0.4f);

	//ゴールしたとき用　タイムボード見るやつ
	void Goal(vec *TimeBoardPos);

	//好きなとこを見る用
	void FreeLook(vec *p, vec *d, vec *u,
					float posk=50.0f, float dirk=0.0f, float atk=1.4f);

	//各シーンのカメラの動きをうんたら
	void CourseEdit(vec *pos, float CamLen);
	void CourseEdit2(vec *p, mat4 *rota, float CamLen);
	void Menu(vec *p, vec *at, vec *u,
					float posk=50.0f, float dirk=0.0f, float atk=1.4f);
	//void Menu(vec *p, POSINDEX *idx, vec *at, vec *u,
	//				float posk=50.0f, float dirk=0.0f, float atk=1.4f);

	//PAS用カメラ　テストでvecとvec3の両方作成
	void PASCamera(vec BasePoint, vec Direc, vec UP, float Length);
	void PASCamera(vec3* BasePoint, vec3* Direc, vec3* UP, float Length);
	//Menu2();
	//	･･

	// シェーダ講座用カメラ操作
	void ShaderLecture();

	//カメラを移動させる
	void CameraMove(vec* direct);

	//フリールック処理　カメラを回転させる 外用と中用
	void SetCameraRotaX(FLOAT value, int flag);
	void SetCameraRotaX_inside( FLOAT xValue);
	void SetCameraRotaX_outside( FLOAT xValue);
	void SetCameraRotaY(FLOAT value, int flag);
	void SetCameraRotaY_inside( FLOAT yValue);
	void SetCameraRotaY_outside( FLOAT yValue);

	//画角の自動設定関数
	void SetMenuMode();
	void SetGameMode(){}

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	void ResetFreeCamera();
	
	mat4*	GetView(){return &view;}
	mat4*	GetPers(){ return &pers; }

	vec3*	GetCameraPos()	{return &pos;}
	vec3*	GetCameraDir()	{ return &dir; }
	vec3*	GetCameraUp()	{ return &up; }
	float			GetCameraLen()	{return len;}
	//POSINDEX*		GetPosIndex()	{return &posidx;}
	void	SetCameraPos(vec3* p){ memcpy_s(&pos,sizeof(vec3),p,sizeof(vec3)); }
	void	SetCameraDir(vec3* p){ memcpy_s(&dir, sizeof(vec3), p, sizeof(vec3)); }
	void	SetCameraUp(vec3* p){ memcpy_s(&up, sizeof(vec3), p, sizeof(vec3)); }
	//void	SetCameraIdx(POSINDEX* p)	{memcpy(&posidx ,p,sizeof(POSINDEX));}
	//void	SetCameraIdx(ONEOF_PI x,ONEOF_PI y,ONEOF_PI z);

	FLOAT GetCameraX(){return cameraX;}
	FLOAT GetCameraY(){return cameraY;}

	void SetAspect(float width, float height);
	void DebugDraw(DebugCommentManager* pDebugM);

	///////////////////////////////////////////////////////////////////////
	//void TransPosIdx(int Flag, int sign); //sign only 1 or -1
	//inline void Transfunction(float* pos, ONEOF_PI* idx, int flag);
	//inline void TransfunctionOwnCamera(float* pos, ONEOF_PI* idx);
	//void CheckTransPosIndex(D3DXVECTOR3* pPos, POSINDEX* pIdx);
	//void CheckTransPIOwnCamera();
	void ResetCamera();
};

#endif
