#include"CMatrix33.h"
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
/*
m[0][0] m[0][1] m[0][2]
m[1][0] m[1][1] m[1][2]
m[2][0] m[2][1] m[2][2]
*/
void CMatrix33::Print() {
	//	for (int i = 0; i < 3; i++){
	//		for (int j = 0; j < 3; j++){
	printf("%f %f %f\n%f %f %f\n%f %f %f\n", m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
	//		}
	//		printf("\n");
	//	}
}
void CMatrix33::Identity() {
	memset(m, 0, sizeof(m));
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}
void CMatrix33::SetRotation(float degree) {
	float radian = degree * M_PI / 180.0f;
	float sin = sinf(radian);
	float cos = cosf(radian);
	Identity();
	m[0][0] = m[1][1] = cos;
	m[0][1] = -sin;
	m[1][0] = sin;
}
const CVector2 CMatrix33::MultiVector2(const CVector2 &v) const
{
	CVector2 vector;
	vector.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];

	vector.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];

	return vector;
}
const CVector2 CMatrix33::operator*(const CVector2 &v) const {
	return MultiVector2(v);
}
void CMatrix33::SetTranslate(float x, float y) {
	Identity();
	m[0][2] = x;
	m[1][2] = y;
}
const CMatrix33 CMatrix33::operator*(const CMatrix33 &a)const
{
return MultiMatrix33(a); 
}
const CMatrix33 CMatrix33::MultiMatrix33(const CMatrix33 &a) const {
	CMatrix33 r;
	r.m[0][0] = m[0][0] * a.m[0][0] + m[0][1] * a.m[1][0] + m[0][2] * a.m[2][0];
	r.m[0][1] = m[0][0] * a.m[0][1] + m[0][1] * a.m[1][1] + m[0][2] * a.m[2][1];
	r.m[0][2] = m[0][0] * a.m[0][2] + m[0][1] * a.m[1][2] + m[0][2] * a.m[2][2];

	r.m[1][0] = m[1][0] * a.m[0][0] + m[1][1] * a.m[1][0] + m[1][2] * a.m[2][0];
	r.m[1][1] = m[1][0] * a.m[0][1] + m[1][1] * a.m[1][1] + m[1][2] * a.m[2][1];
	r.m[1][2] = m[1][0] * a.m[0][2] + m[1][1] * a.m[1][2] + m[1][2] * a.m[2][2];

	r.m[2][0] = m[2][0] * a.m[0][0] + m[2][1] * a.m[1][0] + m[2][2] * a.m[2][0];
	r.m[2][1] = m[2][0] * a.m[0][1] + m[2][1] * a.m[1][1] + m[2][2] * a.m[2][1];
	r.m[2][2] = m[2][0] * a.m[0][2] + m[2][1] * a.m[1][2] + m[2][2] * a.m[2][2];
	return r;
}
void CMatrix33::SetScale(float x, float y){
	Identity();
	m[0][0] = x;
	m[1][1] = y;
}