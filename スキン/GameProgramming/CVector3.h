#ifndef CVECTOR3_H
#define CVECTOR3_H
#define _USE_MATH_DEFINES
#include <math.h>

//#include "C3DBox.h"
class CMatrix44;
/*
2次元ベクトルクラスの定義
*/
class CVector3 {
public:
	float x;	//x座標
	float y;	//y座標
	float z;

	CVector3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}
	//引数付きのコンストラクタ
	CVector3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{}
	//float Length();
	//CMatrix44との掛け算
	const CVector3 operator * (const CMatrix44 &m) const;
	// +演算子のオーバーロード
	const CVector3 operator+(const CVector3 &v) const;
	void operator += (const CVector3 &v);
	// -演算子のオーバーロード
	const CVector3 operator-(const CVector3 &v) const;
	void operator -= (const CVector3 &v);
	const CVector3 operator * (const CVector3 &v) const;
	void operator *= (const CVector3 &v);
	const CVector3 operator / (const CVector3 &v) const;
	void operator /= (const CVector3 &v);
	// *演算子のオーバーロード
	const CVector3 operator*(float f) const;
	void operator *= (float f);
	const CVector3 operator / (float f) const;
	void operator /= (float f);
	// 内積
	float Dot(const CVector3 &v);
	// 長さを求める
//	float Length();
	/* ベクトルの大きさを返す
	ただし、x,y,zのみ
	*/
	float length();
	/* 正規化したベクトルを返す
	ベクトルの正規化：大きさを1にする
	*/
	CVector3 normalize();
	/* 指定されたベクトルの方向へ向ける為の回転角度を得る
	return:｜X軸の回転角度　Y軸の回転角度　０度｜
	*/
	CVector3 getRotationTowards(const CVector3 &dir);
	/* 指定された半径の球上の位置をランダムに設定する
	ベクトル｜0.0 0.0 1.0｜をX軸でランダムに回転させ、次に
	Y軸でランダムに回転させ、半径を掛けた値を設定する
	*/
	void setRandomPositionSphere(float radius);
	/*外積を求める*/
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