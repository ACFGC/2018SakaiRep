#include "CMaterial.h"
#include "glut.h"
#include"CModel.h"
CMaterial::CMaterial(){}
CMaterial::CMaterial(CModel *model)
: mpName(0)
, mVertexNo(0)
, mTextureId(0)
{
	//CModelXにマテリアルを追加する
	model->mMaterial.push_back(this);

	model->GetToken(); // { ? Name
	if (strcmp(model->mToken, "{") != 0){
		//{でないときはマテリアル名
		mpName = new
			char[strlen(model->mToken) + 1];
		strcpy(mpName, model->mToken);
		model->GetToken(); //{
	}
	mDiffuse[0] = model->GetFloatToken();
	mDiffuse[1] = model->GetFloatToken();
	mDiffuse[2] = model->GetFloatToken();
	mDiffuse[3] = model->GetFloatToken();

	mPower = model->GetFloatToken();

	mSpecular[0] = model->GetFloatToken();
	mSpecular[1] = model->GetFloatToken();
	mSpecular[2] = model->GetFloatToken();

	mEmissive[0] = model->GetFloatToken();
	mEmissive[1] = model->GetFloatToken();
	mEmissive[2] = model->GetFloatToken();

	model->GetToken(); //TextureFilename or }

	if (strcmp(model->mToken, "TextureFilename") == 0){
		//テクスチャありの場合、テクスチャファイル名所得
		model->GetToken(); //{
		model->GetToken(); //filename
		mpTextureFilename = new char[strlen(model->mToken) + 1];
		strcpy(mpTextureFilename, model->mToken);
		//テクスチャファイル名でテクスチャを検索する
		CTexture *p = model->FindTexture(model->mToken);
		//テクスチャが在る時
		//if (p){
		//	//テクスチャIDを所得
		//	mTextureId = p->id;
		//}
		//else{
		//	//無い場合、テクスチャ読み込み
		//	p = new CTexture(model->mToken);
		//	if (p->mpName){
		//		//テクスチャ配列に追加
		//		model->mTexture.push_back(p);
		//		//テクスチャIDを所得
		//		mTextureId = p->id;
		//	}
		//}
		model->GetToken(); // }
		model->GetToken(); // }
	}
}

//CMaterial::~CMaterial() {
//	if (mpName) {
//		delete mpName;
//	}
//}

void CMaterial::SetMaterial() {
	
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmbient);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDiffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSpecular);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mEmission);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mShinness);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecular);
//	glMaterialfv(GL_FRONT, GL_EMISSION, mEmission);
//	glMaterialf(GL_FRONT, GL_SHININESS, mShinness);
	if (mTexture.mId) {
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void CMaterial::UnSetMaterial() {
	if (mTexture.mId) {
		//アルファブレンドを無効
		glDisable(GL_BLEND);
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);
	}
}

void CMaterial::SetTexture(char *file) {
	mTexture.Load(file);
}


