#ifndef C3DBALL_H
#define C3DBALL_H
#include"CParts.h"
#include"glut.h"
#include"CTask.h"
#include"CCollider.h"
class C3DBall : public CTask{
public:
	int mClear;
	void Init();
	void Update();
	void Render();
	int y;
	void SetDiffuse(float r, float g, float b, float a);
	int mGravityV; //�d�͑��x
	CParts mBall;
	void OnCollision(CCollider *p);
	int i;
	const double g = 9.8; /* �d�͉����x */
	double t; /* ���� */
	double x; /* �������� */

};
#endif