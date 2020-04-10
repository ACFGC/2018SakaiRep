#include "CCollider.h"
#include "CCharacter.h"
#include "glut.h"
#include "CSceneGame.h"
#include"CTask.h"

CCollider::CCollider()
: mpParent(0)
, mRadius(0)
{
	CollisionManager.Add(this);
}

CCollider::~CCollider() {
	CollisionManager.Remove(this);
}

void CCollider::Render() {
	glPushMatrix();
	glMultMatrixf(CMatrix().Translate(mpParent->mPosition.mX, mpParent->mPosition.mY, mpParent->mPosition.mZ).mM[0]);

	float c[] = {0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);

	glutWireSphere(mRadius, 16, 16);

	glPopMatrix();
}
bool CSphereCollider::Collision(CSphereCollider *p2){
	CVector3 pos1 = mpTask->mMatrix * mPosition;
	CVector3 pos2 = p2->mpTask->mMatrix * p2->mPosition;
	pos1 = pos2 - pos1;
	if (pos1.length() < mRadius + p2->mRadius){
		mAdjust = pos1 * ((mRadius + p2->mRadius - pos1.length()) / (mRadius + p2->mRadius));
		p2->mAdjust = mAdjust * -1;
		return true;
	}
	else{
		mAdjust = p2->mAdjust = CVector3(0.0f, 0.0f, 0.0f);
	}
	return false;
}
#include "math.h"

