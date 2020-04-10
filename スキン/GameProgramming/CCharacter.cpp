#include "CCharacter.h"
#include"CRectangle.h"
CCharacter::CCharacter()
: mScale(1.0f, 1.0f, 1.0f)
, mpModel(0)
{

}

CCharacter::~CCharacter() {
}
/*
Init
初期化処理
*/
void CCharacter::Init(CModel *model){
	mpModel = model;
	//最初のアニメーションにする
	mAnimationLoopFlg = true;
	//1アニメーション60フレーム
	mAnimationFrameSize = 60.0f;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
	//アニメーションの重みを 1.0(100%)にする
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;
}
/*
ChageAnimation
アニメーションを切り替える
index:アニメーションの番号
loop:true 繰り返す
framesize:最後まで再生するのに使用されるフレーム数
*/

void CCharacter::ChangeAnimation(int index, bool loop, float framesize){
	//同じ場合は切り換えない
	if (mAnimationIndex == index)return;
	//今のアニメーションの重みを0.0(0%)にする
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 0.0f;
	//番号、繰り返し、フレーム数を設定
	mAnimationIndex = index % mpModel->mAnimationSet.size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
	//アニメーションの重みを1.0(100%)にする
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;
}
/*
Update
更新する
matrix:移動、回転、拡大縮小の行列
*/
void CCharacter::Update(CMatrix44 &matrix){
	//最後まで再生していない
	if (mAnimationFrame < mAnimationFrameSize){
		//アニメーションの時間を計算
		mpModel->mAnimationSet[mAnimationIndex]->mTime =
			mpModel->mAnimationSet[mAnimationIndex]->mMaxTime *
			mAnimationFrame / mAnimationFrameSize;
		//フレームを進める
		mAnimationFrame++;
	}
	else{
		//繰り返しの場合は、フレームを０に戻す
		if (mAnimationLoopFlg){
			//アニメーションのフレームを最初にする
			mAnimationFrame = 0.0f;
			mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
		}
	}
	//フレームの変換をアニメーションで更新する
	mpModel->AnimateFrame();
	//フレームの合成行列を計算する
	mpModel->mFrame[0]->AnimateCombined(&matrix);
	//頂点にアニメーションを適用する
	mpModel->AnimateVertex();
}
void CCharacter::Init(CModel *model, float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
{
	mpModel = model;
	mPosition = CVector(px, py, pz);
	mRotation = CVector(rx, ry, rz);
	mScale = CVector(sx, sy, sz);
}

void CCharacter::Update() {
	mMatrix = CMatrix().Scale(mScale.mX, mScale.mY, mScale.mZ);
	mMatrixRotation = CMatrix().RotateZ(mRotation.mZ);
	mMatrixRotation = mMatrixRotation * CMatrix().RotateX(mRotation.mX);
	mMatrixRotation = mMatrixRotation * CMatrix().RotateY(mRotation.mY);
	mMatrix = mMatrix * mMatrixRotation * CMatrix().Translate(mPosition.mX, mPosition.mY, mPosition.mZ);
	CMatrix44 pos, rot;
	//位置の行列を所得
	pos.translate(mPosition2);
	//回転（Y軸）の行列を所得
	rot.SetRotationY(mRotation2.y);
	//位置回転行列計算
	mMatrix44 = pos * rot;
	//アニメーションを更新する
	Update(mMatrix44);
}

void CCharacter::Render() {
	mpModel->Render(mMatrix);
}
//void CCharacter::Collision(CCollider *p) {
//	if (p->mpTask->mTaskTag == EMATERIAL){
//		p->mPosition = mPosition;
//		CTaskManager::Get()->Add(p);
//	}
//}