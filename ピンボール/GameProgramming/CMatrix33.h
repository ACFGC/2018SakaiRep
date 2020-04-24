#ifndef CMATRIX33_H
#define CMATRIX33_H
#include <string.h>	//memset�̃C���N���[�h
//2�����x�N�g���N���X�̃C���N���[�h
#include "CVector2.h"

/*
 �s��N���X�̒�`
*/
class CMatrix33 {
public:
	//3�~3�̍s��
	float m[3][3];

	CMatrix33() {
		//�P�ʍs���ݒ�
		Identity();
	}
	//�P�ʍs���ݒ肷��
	void Identity();
	//�s��̏o��
	void Print();
	//��]�s��̐ݒ�
	void SetRotation(float degree);
	//4
	//�ړ��s��̐ݒ�
	void SetTranslate(float x, float y);
	//2�����x�N�g���N���X���|����
	const CVector2 MultiVector2(const CVector2 &v) const;
	// *���Z�q�̃I�[�o�[���[�h
	const CVector2 operator*(const CVector2 &v) const;

	//3�s3��s��N���X���|����
	const CMatrix33 MultiMatrix33(const CMatrix33 &a) const;
	// *���Z�q�̃I�[�o�[���[�h
	const CMatrix33 operator*(const CMatrix33 &a) const;
	void SetScale(float x, float y);
};

#endif
