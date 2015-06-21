#ifndef _CAMERA_H_
#define _CAMERA_H_
//////////////////////////////////////
/*			�J�����N���X
�N���X���Ǝ��ɃJ�����̏�������
�o�l�J�����@�\����
/////////////////////////////////////*/

//class Camera;

#include"IncludeAll.h"
#include"DebugCommentManager.h"
//#include"_BaseObject.h"

//�J�����̒���
#define CAMERA_FOLLOWOBJ_INSIDE		0
#define CAMERA_FOLLOWOBJ_OUTSIDE	1


class Camera{

	mat4		view; 
	mat4		pers;
	//POSINDEX	posidx;

	//�o�l�J�����p�ϐ��Q
	FLOAT	len;	//�J�����ʒu���王�_�����ւǂꂾ�����炷��
	vec3	pos;	//�J�����̈ʒu
	vec3	CamRot; //�t���[���b�N�J�����̌���
	vec3	dir;	//�J�����̎��_�x�N�g��
	vec3	up;		//�J�����̏����
	vec3	v_pos;	//�J�����̈ʒu�X�s�[�h
	vec3	v_dir;	//�J�����̎��_�X�s�[�h
	vec3	v_up;	//�J�����̏�����X�s�[�h

	//�t���[���b�N�J�����p�ϐ�
	FLOAT	cameraX; //�t���[���b�N�J�����̌����̉�]�p�x
	FLOAT	cameraY; //�t���[���b�N�J�����̌����̉�]�p�x

	float ViewportWidth;	// �Q�[����ʂ̉��̒���
	float ViewportHeight;	// �Q�[����ʂ̏c�̒���
	D3DVIEWPORT9 ViewData;	// �r���[�|�[�g�ݒ�p�\����

	void (Camera::*SetCameraRotaFunc[6])(FLOAT);
	void (Camera::*FollowObjectFunc[3])(vec *p, vec *d, vec *u);

public:
	Camera();
	void Reset();

	//���������e��s����f�o�C�X�ɃZ�b�g
	void SetTransform(DXDEVICE* pd3ddev);
	//���v���C�l���ƃJ�����ԍ�����AspectRatio��Viewport�̐ݒ�
	void SetViewPort(int playerNum, int CameraNum, float WindW, float WindH);

	//pos,dir,up��ݒ�
	void SetView(vec *pos, vec *dir, vec *up);
	void SetView(vec3 *pos, vec3 *dir, vec3 *up);
	//View�s����Z�b�g
	void SetView();
	//�ˉe�ϊ��s����Z�b�g
	void SetPers(float fovYRad, float znear, float zfar);

	//�o�l�J�����̂���@�o�l�J�����p�ϐ��̍X�V
	void SetViewLerp(vec *POS, vec *DIR, vec *UP,
			FLOAT len, FLOAT posk=0.93f, FLOAT dirk=0.07f, FLOAT upk=0.1f);
	//���������ق�Ƃ̃o�l�J����
	void SetViewSpring(vec *POS, vec *DIR, vec *UP,
			FLOAT len,FLOAT posk=20.93f, FLOAT dirk=0.75f, FLOAT upk=1.5f);
	//�t���[���b�N�J�����̌v�Z�i�o�l����j
	void FreeLookCameraCalc(vec3* Out, vec* OrgDir,	vec* x, vec* y);
	
	///////////////////////////////////////////////////////////////////////
	/////////////////////////�J�����R���g���[���[//////////////////////////
	/////////////////////�eMove()�̈�ԍŌ�ŌĂяo��//////////////////////
	//�I�u�W�F�N�g�ɂ��Ă䂭
	void FollowObject(vec* pPos, mat4* rota, int CameraLen);
	void FollowObject_inside(vec *p, vec *d, vec *u);
	void FollowObject_outside(vec *p, vec *d, vec *u);

	//�I�u�W�F�N�g���݂邾���@�J�����̈ړ��͂��Ȃ�
	//void LookObject(_BaseObject *pObj);
	void LookObject(vec *p, float posk = 50.0f, float dirk = 0.0f, float atk = 0.4f);

	//�S�[�������Ƃ��p�@�^�C���{�[�h������
	void Goal(vec *TimeBoardPos);

	//�D���ȂƂ�������p
	void FreeLook(vec *p, vec *d, vec *u,
					float posk=50.0f, float dirk=0.0f, float atk=1.4f);

	//�e�V�[���̃J�����̓��������񂽂�
	void CourseEdit(vec *pos, float CamLen);
	void CourseEdit2(vec *p, mat4 *rota, float CamLen);
	void Menu(vec *p, vec *at, vec *u,
					float posk=50.0f, float dirk=0.0f, float atk=1.4f);
	//void Menu(vec *p, POSINDEX *idx, vec *at, vec *u,
	//				float posk=50.0f, float dirk=0.0f, float atk=1.4f);

	//PAS�p�J�����@�e�X�g��vec��vec3�̗����쐬
	void PASCamera(vec BasePoint, vec Direc, vec UP, float Length);
	void PASCamera(vec3* BasePoint, vec3* Direc, vec3* UP, float Length);
	//Menu2();
	//	��

	// �V�F�[�_�u���p�J��������
	void ShaderLecture();

	//�J�������ړ�������
	void CameraMove(vec* direct);

	//�t���[���b�N�����@�J��������]������ �O�p�ƒ��p
	void SetCameraRotaX(FLOAT value, int flag);
	void SetCameraRotaX_inside( FLOAT xValue);
	void SetCameraRotaX_outside( FLOAT xValue);
	void SetCameraRotaY(FLOAT value, int flag);
	void SetCameraRotaY_inside( FLOAT yValue);
	void SetCameraRotaY_outside( FLOAT yValue);

	//��p�̎����ݒ�֐�
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
