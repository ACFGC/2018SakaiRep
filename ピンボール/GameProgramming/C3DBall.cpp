#include"C3DBall.h"
#include"windows.h"
#include"CMain.h"
#include"CRectangle.h"


//緑
//GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
//ライトの位置
//GLfloat lightpos[] = { 200.0, 150.0, -500.0, 1.0 };

//CMatrix44 Matrix;
#define GRAVITY 1
extern CTexture Texture;
void C3DBall::Init(){
	mpSphereCollider  = new CSphereCollider();
	mpSphereCollider->mpTask = this;
	CCollisionManager::Get()->Add(mpSphereCollider);
	mpSphereCollider->mRadius = 80;
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	mGravityV = 0;
	mClear = 0;
	//mClear;
}
void C3DBall::Update(){
	mBall.mPosition.y -= 30.1f;
//	mBall.mPosition.y += mGravityV;
	mBall.Update();
	CTask::mMatrix = mBall.mMatrix;
	CCollisionManager::Get()->Collision(mpSphereCollider);
//	if (mGravityV < -GRAVITY)
	//重力速度分移動
//	y += mGravityV;
	//重力速度更新
	mGravityV -= GRAVITY;



}
void C3DBall::Render(){
	mBall.Render();
	//行列設定
	glPushMatrix();
	glMultMatrixf(CTask::mMatrix.Transpose().f);
	glutSolidSphere(100.0, 32, 32);
	//glMultMatrixf(CTask::mMatrix.f);
	glPopMatrix();
}
void C3DBall::SetDiffuse(float r, float g, float b, float a){
	mBall.SetDiffuse(r, g, b, a);
}
void C3DBall::OnCollision(C3DCollider *p){
	switch (p->mpTask->mTaskTag){
	default:
		mBall.mPosition = mBall.mPosition + p->mAdjust;
//		mGravityV = 0;
		break;
	}


	if (p->mpTask->mTaskTag == EGOAL){

		if (mBall.mPosition.z>= EGOAL){
			mClear++;
			CMain::mSceneTag = CScene::EWIN;
		}
	}
}

C3DBall* C3DBall::sbBall = NULL;