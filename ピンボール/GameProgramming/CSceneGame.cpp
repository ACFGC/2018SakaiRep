#include "CSceneGame.h"
#include "glut.h"
#include "CTriangle.h"
#include "CKey.h"
#include "CFire.h"
#include <math.h>
#include"stage.h"


CTaskManager TaskManager;
CCollisionManager CollisionManager;

void CSceneGame::Init() {
	//mModel.Load("f14.obj", "f14.mtl");
	//mSky.Load("sky.obj", "sky.mtl");
	mF16.Load("f16.obj", "f16.mtl");
	mstage.Load("��.obj", "��.mtl");
	mstage2.Load("�y��.obj", "�y��.mtl");
	mstage3.Load("�J�[�u.obj", "�J�[�u.mtl");
	mstage4.Load("����.obj", "����.mtl");
	mstage5.Load("���o�[�E.obj", "���o�[�E.mtl");
	mstage6.Load("���o�[��.obj", "���o�[��.mtl");
	mstage7.Load("�����o��.obj", "�����o��.mtl");
	mModel.Load("�{�[��.obj", "�{�[��.mtl");
	Ball.Init(&mModel, -3.0f, -3.0f, -28.0f, 0.0f, 0.0f, 0.0f, 0.32f, 0.3f, 0.3f);
	//new CEnemy(&mF16, 0.0f, 5.0f, 4.0f, 0.0f, 90.0f, -30.0f, 0.2f, 0.2f, 0.2f);
	//new CEnemy(&mF16, 0.0f, 7.0f, 6.0f, 0.0f, 90.0f, -30.0f, 0.2f, 0.2f, 0.2f);
	//age(&mstage, 0.0f, 0.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	new stage(&mstage2, 0.0f, -3.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	//new stage(&mstage7, 0.0f, -3.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	//new stage(&mstage3, 0.0f, 0.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	new stage(&mstage4, 0.0f, 0.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	mBar.Init(&mstage7, 0.0f, -3.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	mPin2.Init(&mstage5, -5.0f, 0.0f, -25.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	mPin.Init(&mstage6, 5.0f, 0.0f, -25.0f, 0.0f, -90.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	//new curve(&mcurve, 20.0f, 4.3f, 0.0f, 0.0f, -270.0f, 180.0f, 0.5f, 0.5f, 0.5f);

}

void CSceneGame::Update() {

	mPin.Update();
	mPin2.Update();
	Ball.Update();
	mBar.Update();

	TaskManager.Update();
	CollisionManager.Update();
	TaskManager.Delete();
	CVector3 eye, target;
	mCamera.mEye = mstage2.mPosition + CVector(0.0f, 55.0f, -5.0f) + mstage2.mMatrixRotation;
	mCamera.mCenter = mstage2.mPosition;
	//mCamera.mUp = CVector(2.0f, 0.0f, 0.0f) * mModel.mMatrixRotation;
	mCamera.Render();
	//gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, 1.0f, 1.0f, 1.0f);

	//mSky.Render(CMatrix());
	mPin.Render();
	mPin2.Render();
	Ball.Render();
	mBar.Render();
	TaskManager.Render();

}

//�f�X�g���N�^
CSceneGame::~CSceneGame() {
	TaskManager.Destory();
}
