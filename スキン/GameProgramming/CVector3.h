#ifndef CVECTOR3_H
#define CVECTOR3_H
#define _USE_MATH_DEFINES
#include <math.h>

//#include "C3DBox.h"
class CMatrix44;
/*
2�����x�N�g���N���X�̒�`
*/
class CVector3 {
public:
	float x;	//x���W
	float y;	//y���W
	float z;

	CVector3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}
	//�����t���̃R���X�g���N�^
	CVector3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{}
	//float Length();
	//CMatrix44�Ƃ̊|���Z
	const CVector3 operator * (const CMatrix44 &m) const;
	// +���Z�q�̃I�[�o�[���[�h
	const CVector3 operator+(const CVector3 &v) const;
	void operator += (const CVector3 &v);
	// -���Z�q�̃I�[�o�[���[�h
	const CVector3 operator-(const CVector3 &v) const;
	void operator -= (const CVector3 &v);
	const CVector3 operator * (const CVector3 &v) const;
	void operator *= (const CVector3 &v);
	const CVector3 operator / (const CVector3 &v) const;
	void operator /= (const CVector3 &v);
	// *���Z�q�̃I�[�o�[���[�h
	const CVector3 operator*(float f) const;
	void operator *= (float f);
	const CVector3 operator / (float f) const;
	void operator /= (float f);
	// ����
	float Dot(const CVector3 &v);
	// ���������߂�
//	float Length();
	/* �x�N�g���̑傫����Ԃ�
	�������Ax,y,z�̂�
	*/
	float length();
	/* ���K�������x�N�g����Ԃ�
	�x�N�g���̐��K���F�傫����1�ɂ���
	*/
	CVector3 normalize();
	/* �w�肳�ꂽ�x�N�g���̕����֌�����ׂ̉�]�p�x�𓾂�
	return:�bX���̉�]�p�x�@Y���̉�]�p�x�@�O�x�b
	*/
	CVector3 getRotationTowards(const CVector3 &dir);
	/* �w�肳�ꂽ���a�̋���̈ʒu�������_���ɐݒ肷��
	�x�N�g���b0.0 0.0 1.0�b��X���Ń����_���ɉ�]�����A����
	Y���Ń����_���ɉ�]�����A���a���|�����l��ݒ肷��
	*/
	void setRandomPositionSphere(float radius);
	/*�O�ς����߂�*/
	CVector3 Cross(const CVector3 &v) const {
		return CVector3(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x).normalize();

	}

	//CVector3 CVector3::GetRotation(){
	//	CVector3 rot;
	//	rot.y = -(atan2f(z, x)) * 180.0f / M_PI;
	//	rot.x = -(atan2f(y, sqrtf(x*x + z*z))) * 180.0f / M_PI;
	//	return rot;

	//}

};


#endif