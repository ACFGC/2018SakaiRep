#ifndef CVECTOR2_H
#define CVECTOR2_H
/*
 2�����x�N�g���N���X�̒�`
*/
class CVector2 {
public:
	float x;	//x���W
	float y;	//y���W

	CVector2()
		: x(0.0f)
		, y(0.0f)
	{}
	//�����t���̃R���X�g���N�^
	CVector2(float x, float y)
		: x(x)
		, y(y)
	{}

	// +���Z�q�̃I�[�o�[���[�h
	const CVector2 operator+(const CVector2 &v) const;
	// -���Z�q�̃I�[�o�[���[�h
	const CVector2 operator-(const CVector2 &v) const;
	// *���Z�q�̃I�[�o�[���[�h
	const CVector2 operator*(float f) const;
	// ����
	float dot(const CVector2 &v);
	// ���������߂�
	float Length();

};


#endif
