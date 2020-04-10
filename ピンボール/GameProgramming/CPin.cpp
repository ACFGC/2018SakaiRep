#include "CPin.h"
#include "CKey.h"
#include "CSceneGame.h"

CPin *CPin::mpPin = 0;

CPin::CPin() {
	mpPin = this;
}

CPin::~CPin() {
	mpPin = 0;
}

void CPin::Update() {

	//if (CKey::Push('I')) {
	//	mPosition = mPosition + CVector(0.0f, 0.0f, 0.2f) * mMatrixRotation;
	//}
	if (CKey::Push('A')) {
		mRotation.mY++;
		//		mRotation.mZ--;
	}
	if (CKey::Push('D')) {
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