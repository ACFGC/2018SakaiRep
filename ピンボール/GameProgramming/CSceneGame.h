#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CRectangle.h"
#include "CMatrix.h"
#include "CBox.h"
#include "CModel.h"
#include "CCamera.h"
#include "CCharacter.h"
#include"CBall.h"
#include"CPin.h"
#include"CPin2.h"
#include"CBar.h"
#include"CCollider.h"
#include"CCollisionManager.h"

/*
ゲームのシーン
*/
//CSceneクラスを継承する
class CSceneGame : public CScene {
public:
	CModel mModel;
	CModel mSky;
	CModel mF16;
	CModel mstage;
	CModel mstage2;
	CModel mstage3;
	CModel mstage4;
	CModel mstage5;
	CModel mstage6;
	CModel mstage7;
	CBall Ball;
	CCamera mCamera;
	CPin mPin;
	CPin2 mPin2;
	CBar mBar;


	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
	//デストラクタの宣言
	~CSceneGame();
};

extern CTaskManager TaskManager;
extern CCollisionManager CollisionManager;

#endif
