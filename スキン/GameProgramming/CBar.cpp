#include "CBar.h"
#include "CKey.h"

#include "CSceneGame.h"


CBar *CBar::mpBar = 0;

CBar::CBar() {
	mpBar = this;
}

CBar::~CBar() {
	mpBar = 0;
}

void CBar::Update() {

	//if (CKey::Push('I')) {
	//	mPosition = mPosition + CVector(0.0f, 0.0f, 0.2f) * mMatrixRotation;
	//}
	if (CKey::Push('K')) {
		mPosition.mZ++;
		//		mRotation.mZ--;
	}
	if (CKey::Push('I')) {
		mPosition.mZ--;
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