#ifndef CVECTOR2_H
#define CVECTOR2_H
/*
 2次元ベクトルクラスの定義
*/
class CVector2 {
public:
	float x;	//x座標
	float y;	//y座標

	CVector2()
		: x(0.0f)
		, y(0.0f)
	{}
	//引数付きのコンストラクタ
	CVector2(float x, float y)
		: x(x)
		, y(y)
	{}

	// +演算子のオーバーロード
	const CVector2 operator+(const CVector2 &v) const;
	// -演算子のオーバーロード
	const CVector2 operator-(const CVector2 &v) const;
	// *演算子のオーバーロード
	const CVector2 operator*(float f) const;
	// 内積
	float dot(const CVector2 &v);
	// 長さを求める
	float Length();

};


#endif
