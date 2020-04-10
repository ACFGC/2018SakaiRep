#include "CMatrix44.h"
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>

void CMatrix44::Print(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			printf("%9f", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void CMatrix44::Identity() {
	memset(m, 0, sizeof(m));
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}
void CMatrix44::SetRotationX(float degree) {
	float radian = degree * M_PI / 180.0f;
	float sin = sinf(radian);
	float cos = cosf(radian);
	Identity();
	m[1][1] = cos;
	m[1][2] = -sin;
	m[2][1] = sin;
	m[2][2] = cos;
}
void CMatrix44::SetRotationY(float degree) {
	float radian = degree * M_PI / 180.0f;
	float sin = sinf(radian);
	float cos = cosf(radian);
	Identity();
	m[0][0] = cos;
	m[0][2] = sin;
	m[2][0] = -sin;
	m[2][2] = cos;
}

void CMatrix44::SetRotationZ(float degree) {
	float radian = degree * M_PI / 180.0f;
	float sin = sinf(radian);
	float cos = cosf(radian);
	Identity();
	m[0][0] = cos;
	m[0][1] = -sin;
	m[1][0] = sin;
	m[1][1] = cos;
}
void CMatrix44::SetTranslate(const CVector3 &v)
{
	Identity();
	m[0][3] = v.x;
	m[1][3] = v.y;
	m[2][3] = v.z;
}
const CVector3 CMatrix44::MultiVector3(const CVector3 &v) const
{
	CVector3 vector;
	vector.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
	vector.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
	vector.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];

	return vector;
}
const CVector3 CMatrix44::operator*(const CVector3 &v) const {
	return MultiVector3(v);
}
const CMatrix44 CMatrix44::operator*(const CMatrix44 &a)const
{
	CMatrix44 r;
	r.m[0][0] = m[0][0] * a.m[0][0] + m[0][1] * a.m[1][0] + m[0][2] * a.m[2][0] + m[0][3] * a.m[3][0];
	r.m[0][1] = m[0][0] * a.m[0][1] + m[0][1] * a.m[1][1] + m[0][2] * a.m[2][1] + m[0][3] * a.m[3][1];
	r.m[0][2] = m[0][0] * a.m[0][2] + m[0][1] * a.m[1][2] + m[0][2] * a.m[2][2] + m[0][3] * a.m[3][2];
	r.m[0][3] = m[0][0] * a.m[0][3] + m[0][1] * a.m[1][3] + m[0][2] * a.m[2][3] + m[0][3] * a.m[3][3];

	r.m[1][0] = m[1][0] * a.m[0][0] + m[1][1] * a.m[1][0] + m[1][2] * a.m[2][0] + m[1][3] * a.m[3][0];
	r.m[1][1] = m[1][0] * a.m[0][1] + m[1][1] * a.m[1][1] + m[1][2] * a.m[2][1] + m[1][3] * a.m[3][1];
	r.m[1][2] = m[1][0] * a.m[0][2] + m[1][1] * a.m[1][2] + m[1][2] * a.m[2][2] + m[1][3] * a.m[3][2];
	r.m[1][3] = m[1][0] * a.m[0][3] + m[1][1] * a.m[1][3] + m[1][2] * a.m[2][3] + m[1][3] * a.m[3][3];

	r.m[2][0] = m[2][0] * a.m[0][0] + m[2][1] * a.m[1][0] + m[2][2] * a.m[2][0] + m[2][3] * a.m[3][0];
	r.m[2][1] = m[2][0] * a.m[0][1] + m[2][1] * a.m[1][1] + m[2][2] * a.m[2][1] + m[2][3] * a.m[3][1];
	r.m[2][2] = m[2][0] * a.m[0][2] + m[2][1] * a.m[1][2] + m[2][2] * a.m[2][2] + m[2][3] * a.m[3][2];
	r.m[2][3] = m[2][0] * a.m[0][3] + m[2][1] * a.m[1][3] + m[2][2] * a.m[2][3] + m[2][3] * a.m[3][3];

	r.m[3][0] = m[3][0] * a.m[0][0] + m[3][1] * a.m[1][0] + m[3][2] * a.m[2][0] + m[3][3] * a.m[3][0];
	r.m[3][1] = m[3][0] * a.m[0][1] + m[3][1] * a.m[1][1] + m[3][2] * a.m[2][1] + m[3][3] * a.m[3][1];
	r.m[3][2] = m[3][0] * a.m[0][2] + m[3][1] * a.m[1][2] + m[3][2] * a.m[2][2] + m[3][3] * a.m[3][2];
	r.m[3][3] = m[3][0] * a.m[0][3] + m[3][1] * a.m[1][3] + m[3][2] * a.m[2][3] + m[3][3] * a.m[3][3];

	return r;
	//return MultiMatrix44(a);
}
const CMatrix44 CMatrix44::Transpose()const
{

	CMatrix44 tmp;
	//CMatrix44 i, j;
	//CMatrix44 row, column;
	//CMatrix44 trans[4][4];
	for (int i = 0; i<4; ++i){
		for (int j = 0; j<4; ++j){
			tmp.m[j][i] = m[i][j];
		}
	}
	return tmp;
}
//s—ñ“¯Žm‚ÌŠ|‚¯ŽZ
//const CMatrix44 CMatrix44::operator * (const CMatrix44 &r) const {
//	CMatrix44 tmp;
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			tmp.m[i][j] = 0.0f;
//			for (int k = 0; k < 4; k++) {
//				tmp.m[i][j] += r.m[i][k] * m[k][j];
//			}
//		}
//	}
//	return tmp;
//}
/* ƒxƒNƒgƒ‹‚©‚çˆÚ“®s—ñ‚ðì¬‚·‚é
vFƒxƒNƒgƒ‹bx,y,zb
*/
void CMatrix44::translate(CVector4 v) {
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
}

void CMatrix44::translate(CVector3 v) {
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
}
void CMatrix44::SetQuaternion(float x, float y, float z, float w) {
	m00 = 1 - 2 * y*y - 2 * z*z;	m01 = 2 * x*y + 2 * w*z;		m02 = 2 * x*z - 2 * w*y;		m03 = 0;
	m10 = 2 * x*y - 2 * w*z;		m11 = 1 - 2 * x*x - 2 * z*z;	m12 = 2 * y*z + 2 * w*x;		m13 = 0;
	m20 = 2 * x*z + 2 * w*y;		m21 = 2 * y*z - 2 * w*x;		m22 = 1 - 2 * x*x - 2 * y*y;	m23 = 0;
	m30 = 0;						m31 = 0;						m32 = 0;						m33 = 1;
}
//s—ñ“¯Žm‚ÌŠ|‚¯ŽZ
const CMatrix44 CMatrix44::operator * (float f) const {
	CMatrix44 tmp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp.m[i][j] = m[i][j] * f;
		}
	}
	return tmp;
}

void CMatrix44::operator += (const CMatrix44 &r) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] += r.m[i][j];
		}
	}
}

const CMatrix44 CMatrix44::operator + (const CMatrix44 &r) const {
	CMatrix44 tmp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp.m[i][j] = r.m[i][j] + m[i][j];
		}
	}
	return tmp;
}
