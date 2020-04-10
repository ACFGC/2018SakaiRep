#ifndef C3DBOX_H
#define C3DBOX_H

#include"C3DTriangle.h"
#include"CMatrix44.h"


class C3DBox{
	
public:
	C3DBox()
		: mVelocity(5.0f)
	{}
	float w;
	float h;
	float d;

	C3DTriangle mTriangle[12];
	void SetWHD(float w, float h, float d);
	void Render();
	CMatrix44 mMatrixRotation;
	CVector3 mRotation;
	CVector3 mPosition;
	void Update();
	CMatrix44 mMatrixTranslate;
	CVector3 mForward;
	float mVelocity;
	float mDiffuse[4];
	void Forward();
	CMatrix44 mMatrix;
	void SetDiffuse(float r, float g, float b, float a);
};
#endif