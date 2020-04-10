#ifndef CMATRIX33_H
#define CMATRIX33_H
#include <string.h>	//memsetのインクルード
//2次元ベクトルクラスのインクルード
#include "CVector2.h"

/*
 行列クラスの定義
*/
class CMatrix33 {
public:
	//3×3の行列
	float m[3][3];

	CMatrix33() {
		//単位行列を設定
		Identity();
	}
	//単位行列を設定する
	void Identity();
	//行列の出力
	void Print();
	//回転行列の設定
	void SetRotation(float degree);
	//4
	//移動行列の設定
	void SetTranslate(float x, float y);
	//2次元ベクトルクラスを掛ける
	const CVector2 MultiVector2(const CVector2 &v) const;
	// *演算子のオーバーロード
	const CVector2 operator*(const CVector2 &v) const;

	//3行3列行列クラスを掛ける
	const CMatrix33 MultiMatrix33(const CMatrix33 &a) const;
	// *演算子のオーバーロード
	const CMatrix33 operator*(const CMatrix33 &a) const;
	void SetScale(float x, float y);
};

#endif
