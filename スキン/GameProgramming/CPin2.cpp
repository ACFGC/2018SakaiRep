#include "CPin2.h"
#include "CKey.h"

#include "CSceneGame.h"


CPin2 *CPin2::mpPin2 = 0;

CPin2::CPin2() {
	mpPin2 = this;
}

CPin2::~CPin2() {
	mpPin2 = 0;
}

void CPin2::Update() {

	//if (CKey::Push('I')) {
	//	mPosition = mPosition + CVector(0.0f, 0.0f, 0.2f) * mMatrixRotation;
	//}
	if (CKey::Push('J')) {
		mRotation.mY++;
		//		mRotation.mZ--;
	}
	if (CKey::Push('L')) {
		mRotation.mY--;
		//		mRotation.mZ++;
	}

	//if (CKey::Push('W')) {
	//	mRotation.mX++;
	//}
	
	//if (CKey::Push('S')) {
	//	mRotation.mX--;
	//}

	//if (CKey::Push(VK_SPACE)) {
	//	CBullet *b = new CBullet();
	//	b->Set(0.0f, 0.0f, 0.0f, 0.03f, 0.03f, 0.3f);
	//	b->SetDiffuse(1.0f, 1.0f, 0.0f, 1.0f);
	//	b->mPosition = mPosition;
	//	b->mRotation = mRotation;
	//	TaskManager.Add(b);
	//}
	CCharacter::Update();
}