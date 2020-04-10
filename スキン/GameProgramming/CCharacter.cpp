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
����������
*/
void CCharacter::Init(CModel *model){
	mpModel = model;
	//�ŏ��̃A�j���[�V�����ɂ���
	mAnimationLoopFlg = true;
	//1�A�j���[�V����60�t���[��
	mAnimationFrameSize = 60.0f;
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
	//�A�j���[�V�����̏d�݂� 1.0(100%)�ɂ���
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;
}
/*
ChageAnimation
�A�j���[�V������؂�ւ���
index:�A�j���[�V�����̔ԍ�
loop:true �J��Ԃ�
framesize:�Ō�܂ōĐ�����̂Ɏg�p�����t���[����
*/

void CCharacter::ChangeAnimation(int index, bool loop, float framesize){
	//�����ꍇ�͐؂芷���Ȃ�
	if (mAnimationIndex == index)return;
	//���̃A�j���[�V�����̏d�݂�0.0(0%)�ɂ���
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 0.0f;
	//�ԍ��A�J��Ԃ��A�t���[������ݒ�
	mAnimationIndex = index % mpModel->mAnimationSet.size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;
}
/*
Update
�X�V����
matrix:�ړ��A��]�A�g��k���̍s��
*/
void CCharacter::Update(CMatrix44 &matrix){
	//�Ō�܂ōĐ����Ă��Ȃ�
	if (mAnimationFrame < mAnimationFrameSize){
		//�A�j���[�V�����̎��Ԃ��v�Z
		mpModel->mAnimationSet[mAnimationIndex]->mTime =
			mpModel->mAnimationSet[mAnimationIndex]->mMaxTime *
			mAnimationFrame / mAnimationFrameSize;
		//�t���[����i�߂�
		mAnimationFrame++;
	}
	else{
		//�J��Ԃ��̏ꍇ�́A�t���[�����O�ɖ߂�
		if (mAnimationLoopFlg){
			//�A�j���[�V�����̃t���[�����ŏ��ɂ���
			mAnimationFrame = 0.0f;
			mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
		}
	}
	//�t���[���̕ϊ����A�j���[�V�����ōX�V����
	mpModel->AnimateFrame();
	//�t���[���̍����s����v�Z����
	mpModel->mFrame[0]->AnimateCombined(&matrix);
	//���_�ɃA�j���[�V������K�p����
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
	//�ʒu�̍s�������
	pos.translate(mPosition2);
	//��]�iY���j�̍s�������
	rot.SetRotationY(mRotation2.y);
	//�ʒu��]�s��v�Z
	mMatrix44 = pos * rot;
	//�A�j���[�V�������X�V����
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