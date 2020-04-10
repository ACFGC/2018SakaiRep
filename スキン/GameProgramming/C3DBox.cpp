#include"C3DBox.h"

void C3DBox::SetWHD(float w, float h, float d){
	this->w = w; this->h = h; this->d = d;
	mTriangle[0].mVertex[0].x = -w;
	mTriangle[0].mVertex[0].y = -h;
	mTriangle[0].mVertex[0].z = d;
	mTriangle[0].mVertex[1].x = w;
	mTriangle[0].mVertex[1].y = -h;
	mTriangle[0].mVertex[1].z = d;
	mTriangle[0].mVertex[2].x = w;
	mTriangle[0].mVertex[2].y = h;
	mTriangle[0].mVertex[2].z = d;
	mTriangle[0].mNormal = CVector3(0.0f, 0.0f, 1.0f);
	mTriangle[1].mVertex[0].x = -w;
	mTriangle[1].mVertex[0].y = h;
	mTriangle[1].mVertex[0].z = d;
	mTriangle[1].mVertex[1].x = -w;
	mTriangle[1].mVertex[1].y = -h;
	mTriangle[1].mVertex[1].z = d;
	mTriangle[1].mVertex[2].x = w;
	mTriangle[1].mVertex[2].y = h;
	mTriangle[1].mVertex[2].z = d;
	mTriangle[1].mNormal = CVector3(0.0f, 0.0f, 1.0f);
	mTriangle[2].mVertex[0].x = w;
	mTriangle[2].mVertex[0].y = -h;
	mTriangle[2].mVertex[0].z = d;
	mTriangle[2].mVertex[1].x = w;
	mTriangle[2].mVertex[1].y = -h;
	mTriangle[2].mVertex[1].z = -d;
	mTriangle[2].mVertex[2].x = w;
	mTriangle[2].mVertex[2].y = h;
	mTriangle[2].mVertex[2].z = -d;
	mTriangle[2].mNormal = CVector3(1.0f, 0.0f, 0.0f);
	mTriangle[3].mVertex[0].x = w;
	mTriangle[3].mVertex[0].y = h;
	mTriangle[3].mVertex[0].z = d;
	mTriangle[3].mVertex[1].x = w;
	mTriangle[3].mVertex[1].y = -h;
	mTriangle[3].mVertex[1].z = d;
	mTriangle[3].mVertex[2].x = w;
	mTriangle[3].mVertex[2].y = h;
	mTriangle[3].mVertex[2].z = -d;
	mTriangle[3].mNormal = CVector3(1.0f, 0.0f, 0.0f);
	mTriangle[4].mVertex[0].x = -w;
	mTriangle[4].mVertex[0].y = h;
	mTriangle[4].mVertex[0].z = d;
	mTriangle[4].mVertex[1].x = w;
	mTriangle[4].mVertex[1].y = h;
	mTriangle[4].mVertex[1].z = d;
	mTriangle[4].mVertex[2].x = w;
	mTriangle[4].mVertex[2].y = h;
	mTriangle[4].mVertex[2].z = -d;
	mTriangle[4].mNormal = CVector3(0.0f, 1.0f, 0.0f);
	mTriangle[5].mVertex[0].x = -w;
	mTriangle[5].mVertex[0].y = h;
	mTriangle[5].mVertex[0].z = -d;
	mTriangle[5].mVertex[1].x = -w;
	mTriangle[5].mVertex[1].y = h;
	mTriangle[5].mVertex[1].z = d;
	mTriangle[5].mVertex[2].x = w;
	mTriangle[5].mVertex[2].y = h;
	mTriangle[5].mVertex[2].z = -d;
	mTriangle[5].mNormal = CVector3(0.0f, 1.0f, 0.0f);
	mTriangle[6].mVertex[0].x = -w;
	mTriangle[6].mVertex[0].y = h;
	mTriangle[6].mVertex[0].z = d;
	mTriangle[6].mVertex[1].x = -w;
	mTriangle[6].mVertex[1].y = h;
	mTriangle[6].mVertex[1].z = -d;
	mTriangle[6].mVertex[2].x = -w;
	mTriangle[6].mVertex[2].y = -h;
	mTriangle[6].mVertex[2].z = -d;
	mTriangle[6].mNormal = CVector3(-1.0f, 0.0f, 0.0f);
	mTriangle[7].mVertex[0].x = -w;
	mTriangle[7].mVertex[0].y = -h;
	mTriangle[7].mVertex[0].z = -d;
	mTriangle[7].mVertex[1].x = -w;
	mTriangle[7].mVertex[1].y = -h;
	mTriangle[7].mVertex[1].z = d;
	mTriangle[7].mVertex[2].x = -w;
	mTriangle[7].mVertex[2].y = h;
	mTriangle[7].mVertex[2].z = d;
	mTriangle[7].mNormal = CVector3(-1.0f, 0.0f, 0.0f);
	mTriangle[8].mVertex[0].x = w;
	mTriangle[8].mVertex[0].y = h;
	mTriangle[8].mVertex[0].z = -d;
	mTriangle[8].mVertex[1].x = w;
	mTriangle[8].mVertex[1].y = -h;
	mTriangle[8].mVertex[1].z = -d;
	mTriangle[8].mVertex[2].x = -w;
	mTriangle[8].mVertex[2].y = -h;
	mTriangle[8].mVertex[2].z = -d;
	mTriangle[8].mNormal = CVector3(0.0f, 0.0f, -1.0f);
	mTriangle[9].mVertex[0].x = -w;
	mTriangle[9].mVertex[0].y = -h;
	mTriangle[9].mVertex[0].z = -d;
	mTriangle[9].mVertex[1].x = -w;
	mTriangle[9].mVertex[1].y = h;
	mTriangle[9].mVertex[1].z = -d;
	mTriangle[9].mVertex[2].x = w;
	mTriangle[9].mVertex[2].y = h;
	mTriangle[9].mVertex[2].z = -d;
	mTriangle[9].mNormal = CVector3(0.0f, 0.0f, -1.0f);

}
void C3DBox::Render(){
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	mTriangle[0].Render();
	mTriangle[1].Render();
	mTriangle[2].Render();
	mTriangle[3].Render();
	mTriangle[4].Render();
	mTriangle[5].Render();
	mTriangle[6].Render();
	mTriangle[7].Render();
	mTriangle[8].Render();
	mTriangle[9].Render();


}
void C3DBox::SetDiffuse(float r, float g, float b, float a){
	mDiffuse[0] = r; mDiffuse[1] = g; mDiffuse[2] = b; mDiffuse[3] = a;
}
void C3DBox::Update(){
	mMatrixRotation.SetRotationY(mRotation.y);
	CMatrix44 rx;
	rx.SetRotationX(mRotation.x);
	mMatrixRotation = mMatrixRotation * rx;
	mMatrixTranslate.SetTranslate(mPosition);
	CMatrix44 rz;
	rz.SetRotationZ(mRotation.z);
	mMatrixRotation = mMatrixRotation * rz;
	mTriangle[0].mMatrix =
	mTriangle[1].mMatrix =
	mTriangle[2].mMatrix =
	mTriangle[3].mMatrix =
	mTriangle[4].mMatrix =
	mTriangle[5].mMatrix =
	mTriangle[6].mMatrix =
	mTriangle[7].mMatrix =
	mTriangle[8].mMatrix =
	mTriangle[9].mMatrix = mMatrixTranslate * mMatrixRotation;
	mForward = mMatrixRotation * CVector3(0.0f, 0.0f, 1.0f);
	mMatrix = mMatrixTranslate * mMatrixRotation;
}
void C3DBox::Forward(){
	mPosition = mPosition + mForward * mVelocity;
}