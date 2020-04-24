#include "CVector2.h"
//�����������߂�֐����C���N���[�h
#include "math.h"

// +���Z�q�̃I�[�o�[���[�h
const CVector2 CVector2::operator+(const CVector2 &v) const {
	return CVector2(x + v.x, y + v.y);
}
// -���Z�q�̃I�[�o�[���[�h
const CVector2 CVector2::operator-(const CVector2 &v) const {
	return CVector2(x - v.x, y - v.y);
}
// *���Z�q�̃I�[�o�[���[�h
const CVector2 CVector2::operator*(float f) const {
	return CVector2(x * f, y * f);
}
// ����
float CVector2::dot(const CVector2 &v) {
	return x*v.x + y*v.y;
}
// ���������߂�
float CVector2::Length() {
	return sqrtf(x*x + y*y);
}

