#ifndef CMATRIX44_H
#define CMATRIX44_H

#include <string.h>	//memsetのインクルード
//2次元ベクトルクラスのインクルード
#include "CVector2.h"
#include"CVector3.h"
#include"CVector4.h"
/* 4x4行列クラスの定義
| f[ 0] f[ 1] f[ 2] f[ 3] |
| f[ 4] f[ 5] f[ 6] f[ 7] |
| f[ 8] f[ 9] f[10] f[11] |
| f[12] f[13] f[14] f[15] |

| m[0][0] m[0][1] m[0][2] m[0][3] |
| m[1][0] m[1][1] m[1][2] m[1][3] |
| m[2][0] m[2][1] m[2][2] m[2][3] |
| m[3][0] m[3][1] m[3][2] m[3][3] |

| m00 m10 m20 m30 |
| m01 m11 m21 m31 |
| m02 m12 m22 m32 |
| m03 m13 m23 m33 |

*/
class CMatrix44{
public:
	union{
		float	f[16];
		float m[4][4];
		struct
		{
			float	m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33;
		};
	};
	CMatrix44(){
		Identity();
	}
	void SetQuaternion(float x, float y, float z, float w);
	//行列同士の掛け算
	//const CMatrix44 operator * (const CMatrix44 &r) const;
	// ベクトルから移動行列を作成するメソッドの宣言
	void translate(CVector4 v);
	void translate(CVector3 v);
	void Identity();
	void SetRotationY(float degree);
	void SetRotationX(float degree);
	void SetRotationZ(float degree);
	void Print();
	const CVector3 MultiVector3(const CVector3 &v) const;
	const CVector3 operator*(const CVector3 &v) const;
	void SetTranslate(const CVector3 &v);
	const CMatrix44 operator*(const CMatrix44 &a)const;
	const CMatrix44 MultiMatrix44(const CMatrix44 &a) const;
	const CMatrix44 Transpose() const;

	const CMatrix44 operator * (float f) const;
	void operator += (const CMatrix44 &r);
	const CMatrix44 operator + (const CMatrix44 &r) const;
};
#endif