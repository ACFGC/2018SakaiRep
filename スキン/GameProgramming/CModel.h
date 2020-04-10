#ifndef CMODEL
#define CMODEL

#define MODEL_FILE "ラグナ.x"

#define SAFE_DELETE_ARRAY(a){ if(a) delete[] a; a = 0;}
#include "CTriangle.h"
#include "CMaterial.h"
#include <string>
#include <vector>
#include"CCollider.h"
#include "CMatrix44.h"
#include "CTriangle.h"
#include"CTexture.h"
#include <stdio.h>
//配列のサイズ所得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
class CModel;
/*
CAnimationKey
アニメーションキークラス
*/
class CAnimationKey{
public:
	//時間
	float mTime;
	//行列
	CMatrix44 mMatrix;
};
/*
CAnimation
アニメーションクラス
*/
class CAnimation{
public:
	char *mpFrameName;//フレーム名
	int mFrameIndex; //フレーム番号

	int mKeyNum; //キー数（数時間数）
	CAnimationKey *mpKey; //キーの配列

	CAnimation(CModel *model);

	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
	}
};
/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet{
public:
	//アニメーション名
	char *mpName;

	float mTime; //現在時間
	float mWeight; //重み
	float mMaxTime; //最大時間



	CAnimationSet(CModel *model);
	//アニメーション
	std::vector<CAnimation*>mAnimation;
	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//アニメーション要素の削除
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}
};
/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights{
public:
	char *mpFrameName; //フレーム名
	int mFrameIndex; //フレーム番号
	int mIndexNum; //頂点番号数
	int *mpIndex; //頂点番号配列
	float *mpWeight; //頂点ウェイト配列
	CMatrix44 mOffset; //オフセットマトリックス

	CSkinWeights(CModel *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};
//CMeshクラスの定義
class CMesh{
public:
	int mVertexNum;//頂点数
	CVector3 *mpVertex; //頂点データ

	int mFaceNum; //面数
	int *mpVertexIndex; //面を構成する頂点番号
	int mNormalNum; //法線数
	CVector3 *mpNormal;//法線データ
	int mMaterialNum; //マテリアル数
	int mMaterialIndexNum; //マテリアル番号数（面数）
	int *mpMaterialIndex;//マテリアル番号
	std::vector<CMaterial*> mMaterial;//マテリアルデータ
	std::vector<CSkinWeights*> mSkinWeights;
	CVector3 *mpAnimateVertex; //アニメーション用頂点
	CVector3 *mpAnimateNormal; //アニメーション用法線
	//テクスチャ座標データ
	CVector2 *mpTextureCoords;
	//コンストラクタ
	CMesh()
		: mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(0)
		, mNormalNum(0)
		, mpNormal(0)
		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(0)
		, mpAnimateVertex(0)
		, mpAnimateNormal(0)
		, mpTextureCoords(0)
	{}
	//デストラクタ
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		SAFE_DELETE_ARRAY(mpAnimateVertex);
		SAFE_DELETE_ARRAY(mpAnimateNormal);
		SAFE_DELETE_ARRAY(mpTextureCoords);
		//スキンウェイトの削除
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	//読み込み処理
	void Init(CModel *model);
	void Render();
	//頂点にアニメーション適用
	void AnimateVertex(CModel *model);

};
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;
	CMatrix44 mTransformMatrix;
	char* mpName;
	int mIndex;
	CMesh mMesh;

	CMatrix44 mCombinedMatrix;

	void AnimateCombined(CMatrix44* parent);
	CModelXFrame(CModel* model);
	void Render();


};
class CModel {
public:
	std::vector<CTriangle*> mTriangles;
	std::vector<CMaterial*> mMaterials;

	float *mpVertex;
	float *mpNormal;
	float GetFloatToken();
	char* mpPointer;
	char mToken[1024];
	float *mpTextureCoord;

	int GetIntToken();
	std::vector<CModelXFrame*>mFrame;
	//アニメーションセットの配列
	std::vector<CAnimationSet*>mAnimationSet;

	std::vector<CMaterial*> mMaterial; //マテリアルの配列

	std::vector<CTexture*> mTexture; //テクスチャの配列
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();

	//マテリアルの検索
	CMaterial* FindMaterial(char* name);

	//テクスチャの検索
	CTexture* FindTexture(char* name);
	CVector mPosition;
	CVector mMatrixRotation;
	CModel();
	~CModel();


	void Load(char *obj, char *mtl);
	void Render(const CMatrix &matrix);
	void Update();
	void Render();
	void AnimateFrame();
	void AnimateVertex();
	void GetToken();
	void SkipNode();
	void Load(char* file);

	// Load from a file
	bool Load(const std::string& fileName);

	/*
	FindFrame
	フレーム名に該当するフレームのアドレスを返す
	*/
	CModelXFrame* CModel::FindFrame(char* name){
		//イテレータの作成
		std::vector<CModelXFrame*>::iterator itr;
		//先頭から最後まで繰り返す
		for (itr = mFrame.begin(); itr != mFrame.end(); itr++){
			//名前が一致したか？
			if (strcmp(name, (*itr)->mpName) == 0){
				//一致したらそのアドレスを返す
				return *itr;
			}
		}
		//一致するフレームが無い場合はNULLを返す
		return NULL;
	}
};


#endif
