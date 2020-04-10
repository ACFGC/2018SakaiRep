#ifndef CCOLLIDER
#define CCOLLIDER

#include "CVector.h"
#include "CTask.h"
class CRectangle;
class CTask;
class CCharacter;
class CSphereCollider;

class CBoxCollider;
class CCircleCollider;

class CCollider : public CTask {
public:
	CTask *mTag;
	//���L��
	CTask *mpTask;
	CVector mV[1];
	float mRadius;
	CCharacter *mpParent;
	////���S���W
	//CVector mPosition;
	CCollider();
	~CCollider();

	void Render();
	static bool Collision(CCollider *, CCollider *);
	////���L��
	//CRectangle *mpTask;
	//���S���W
	CVector2 mPosition;
	//���a
	//�폜float mRadius;
	//�Փ˔���
	static bool Collision(CCircleCollider *p1, CCircleCollider *p2);
	//�Փ˔���(�~�Ǝl�p�`)
	static bool Collision(CCircleCollider *p1, CBoxCollider *p2);
	CVector2 mAdjust;
	//CCollider *mpNext;
	virtual bool Collision(CCircleCollider *p){
		return false;
	};
	virtual bool Collision(CBoxCollider *p){ return false; };

};
class CBoxCollider : public CCollider {
public:
	//x:���̔��� y:�����̔���
	CVector2 mSize;
	bool Collision(CCircleCollider *p);
};
class CCircleCollider : public CCollider{
public:
	float mRadius;
	bool Collision(CCircleCollider *p);
};
class C3DCollider : public CCollider{
public:
	CTask *mpTask;
	CVector3 mPosition;
	CVector3 mAdjust;
	C3DCollider *mpNext;
	virtual bool Collision(CSphereCollider *p){
		return false;
	};
};
class CSphereCollider : public C3DCollider{
public:
	float mRadius;
	bool Collision(CSphereCollider *p);
};
class C3DBoxCollider : public C3DCollider{
public:
	CVector3 mSize;
	bool Collision(CSphereCollider *p);
};
//class CCollisionManager{
//private:
//	static CCollisionManager *mpInstance;
//	C3DCollider *mpHead;
//	C3DCollider *mpTail;
//	CCollisionManager()
//		: mpHead(0)
//		, mpTail(0)
//	{}
//public:
//	static CCollisionManager* Get();
//	void Add(CCollider *p){};
//	void Add(C3DCollider *p);
//	void Collision(CBoxCollider *p);
//	void Collision(CCircleCollider *p);
//	void Collision(CSphereCollider *p);
//	void Destroy();
//	void Remove();
//};

//class CSphereCollider : public CCollider{
//public:
//	CSphereCollider(){
//		mTag = ECOLSPHERE;
//	}
//	CSphereCollider(CCharacter *parent, CMatrix44*matrix, const CVector3 &pos, float radius)
//		: CSphereCollider(){
//		mpParent = parent;
//		mpCombinedMatrix = matrix;
//		mCenter = pos;
//		mRadius = radius;
//	}
//	void Render(){
//		float color[] = { 1.0f, 1.0f, 0.0f, 0.5f };
//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
//		CMatrix44 mat;
//		glPushMatrix();
//		mat.translate(mCenter * *mpCombinedMatrix);
//		glMultMatrixf(mat.f);
//		glutWireSphere(mRadius, 20, 20);
//		glPopMatrix();
//	}
//};


#endif
