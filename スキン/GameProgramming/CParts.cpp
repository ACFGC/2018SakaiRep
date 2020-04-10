#include"CParts.h"

void CParts::Update(){
	if (mpParent){
		mMatrixRotation.SetRotationY(mRotation.y);
		CMatrix44 rx;
		rx.SetRotationX(mRotation.x);
		mMatrixRotation = mMatrixRotation * rx;
		mForward = mMatrixRotation * CVector3(0.0f, 0.0f, 1.0f);
		mMatrixTranslate.SetTranslate(mPosition);
		mMatrix =
			mTriangle[0].mMatrix =
			mTriangle[1].mMatrix =
			mTriangle[2].mMatrix =
			mTriangle[3].mMatrix =
			mTriangle[4].mMatrix =
			mTriangle[5].mMatrix =
			mTriangle[6].mMatrix =
			mTriangle[7].mMatrix =
			mTriangle[8].mMatrix =
			mTriangle[9].mMatrix =

			mpParent->mMatrix * mMatrixTranslate * mMatrixRotation;
	}
	else{
		C3DBox::Update();
	}
}