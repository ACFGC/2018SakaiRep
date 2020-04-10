#ifndef CCHARACTER
#define CCHARACTER

#include "CVector.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CTask.h"
#include"CCollider.h"
#include"CMatrix44.h"
class CCollider;

class CCharacter : public CTask {
public:
	int  i;
	CModel *mpModel; //モデルデータ
	int mAnimationIndex; //アニメーション番号
	bool mAnimationLoopFlg; //ture:繰り返す
	float mAnimationFrame; //アニメーションの再生フレーム
	float mAnimationFrameSize;//アニメーションの再生フレーム数
	//初期化処理
	void Init(CModel *model);
	//アニメーションの変更
	void ChangeAnimation(int index, bool loop, float framesize);
	//衝突処理
	virtual bool Collision(CCollider*, CCollider*){
		return false;
	};
	//キャラクターの識別
	enum ECharTag{
		EPLAYER = 1,//1から順番
		EENEMY,
	};
	ECharTag mTag; //キャラクタ識別タグ
	CVector3 mPosition2; //位置
	CVector3 mRotation2; //回転値
	CMatrix44 mMatrix44; //位置回転行列
	//更新処理
	void Update(CMatrix44 &m);

	CVector mPosition;
	CVector mRotation;
	CVector mScale;
	CMatrix mMatrix;
	CMatrix mMatrixRotation;

	CCharacter();
	~CCharacter();

	void Init(CModel *model, float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
	void Update();
	void Render();
	/*virtual void Collision(CCollider *p, CCollider *y);*/
};

#endif
