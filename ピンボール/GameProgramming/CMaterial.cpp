#include "CMaterial.h"
#include "glut.h"
#include"CModel.h"
CMaterial::CMaterial(){}
CMaterial::CMaterial(CModel *model)
: mpName(0)
, mVertexNo(0)
, mTextureId(0)
{
	//CModelX�Ƀ}�e���A����ǉ�����
	model->mMaterial.push_back(this);

	model->GetToken(); // { ? Name
	if (strcmp(model->mToken, "{") != 0){
		//{�łȂ��Ƃ��̓}�e���A����
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
		//�e�N�X�`������̏ꍇ�A�e�N�X�`���t�@�C��������
		model->GetToken(); //{
		model->GetToken(); //filename
		mpTextureFilename = new char[strlen(model->mToken) + 1];
		strcpy(mpTextureFilename, model->mToken);
		//�e�N�X�`���t�@�C�����Ńe�N�X�`������������
		CTexture *p = model->FindTexture(model->mToken);
		//�e�N�X�`�����݂鎞
		//if (p){
		//	//�e�N�X�`��ID������
		//	mTextureId = p->id;
		//}
		//else{
		//	//�����ꍇ�A�e�N�X�`���ǂݍ���
		//	p = new CTexture(model->mToken);
		//	if (p->mpName){
		//		//�e�N�X�`���z��ɒǉ�
		//		model->mTexture.push_back(p);
		//		//�e�N�X�`��ID������
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
		//�e�N�X�`�����g�p�\�ɂ���
		glEnable(GL_TEXTURE_2D);
		//�e�N�X�`�����o�C���h����
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//�A���t�@�u�����h��L���ɂ���
		glEnable(GL_BLEND);
		//�u�����h���@���w��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void CMaterial::UnSetMaterial() {
	if (mTexture.mId) {
		//�A���t�@�u�����h�𖳌�
		glDisable(GL_BLEND);
		//�e�N�X�`���̃o�C���h������
		glBindTexture(GL_TEXTURE_2D, 0);
		//�e�N�X�`���𖳌��ɂ���
		glDisable(GL_TEXTURE_2D);
	}
}

void CMaterial::SetTexture(char *file) {
	mTexture.Load(file);
}


