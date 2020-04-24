#ifndef CBALL_H
#define CBALL_H
#include"CParts.h"
#include"glut.h"
#include"CTask.h"
#include"CCollider.h"
#include"CCharacter.h"
class CBall : public CCharacter{
public:
	static CBall *mpCBall;
	CBall();
	~CBall();
	int mClear;
	/*void Init();*/
	void Update();
	/*void Render();*/
	void Load(char *obj, char *mtl);
	int y;
	/*void SetDiffuse(float r, float g, float b, float a);*/
	void SetPos(const CVector3& pos);
	int mGravityV; //�d�͑��x
	CParts mBall;
	static CBall* sbBall;
	CSphereCollider *mpSphereCollider;
	void OnCollision(C3DCollider *p);
	void OnCollision(CCollider *p);
	int i;
	const double g = 9.8; /* �d�͉����x */
	double t; /* ���� */
	double x; /* �������� */
	
};
#endif