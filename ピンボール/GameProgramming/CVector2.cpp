#include "CVector2.h"
//平方根を求める関数をインクルード
#include "math.h"

// +演算子のオーバーロード
const CVector2 CVector2::operator+(const CVector2 &v) const {
	return CVector2(x + v.x, y + v.y);
}
// -演算子のオーバーロード
const CVector2 CVector2::operator-(const CVector2 &v) const {
	return CVector2(x - v.x, y - v.y);
}
// *演算子のオーバーロード
const CVector2 CVector2::operator*(float f) const {
	return CVector2(x * f, y * f);
}
// 内積
float CVector2::dot(const CVector2 &v) {
	return x*v.x + y*v.y;
}
// 長さを求める
float CVector2::Length() {
	return sqrtf(x*x + y*y);
}

