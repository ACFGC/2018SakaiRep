#ifndef CMODEL
#define CMODEL

#define MODEL_FILE "���O�i.x"

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
//�z��̃T�C�Y�������}�N����
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
class CModel;
/*
CAnimationKey
�A�j���[�V�����L�[�N���X
*/
class CAnimationKey{
public:
	//����
	float mTime;
	//�s��
	CMatrix44 mMatrix;
};
/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation{
public:
	char *mpFrameName;//�t���[����
	int mFrameIndex; //�t���[���ԍ�

	int mKeyNum; //�L�[���i�����Ԑ��j
	CAnimationKey *mpKey; //�L�[�̔z��

	CAnimation(CModel *model);

	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
	}
};
/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet{
public:
	//�A�j���[�V������
	char *mpName;

	float mTime; //���ݎ���
	float mWeight; //�d��
	float mMaxTime; //�ő厞��



	CAnimationSet(CModel *model);
	//�A�j���[�V����
	std::vector<CAnimation*>mAnimation;
	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//�A�j���[�V�����v�f�̍폜
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}
};
/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights{
public:
	char *mpFrameName; //�t���[����
	int mFrameIndex; //�t���[���ԍ�
	int mIndexNum; //���_�ԍ���
	int *mpIndex; //���_�ԍ��z��
	float *mpWeight; //���_�E�F�C�g�z��
	CMatrix44 mOffset; //�I�t�Z�b�g�}�g���b�N�X

	CSkinWeights(CModel *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};
//CMesh�N���X�̒�`
class CMesh{
public:
	int mVertexNum;//���_��
	CVector3 *mpVertex; //���_�f�[�^

	int mFaceNum; //�ʐ�
	int *mpVertexIndex; //�ʂ��\�����钸�_�ԍ�
	int mNormalNum; //�@����
	CVector3 *mpNormal;//�@���f�[�^
	int mMaterialNum; //�}�e���A����
	int mMaterialIndexNum; //�}�e���A���ԍ����i�ʐ��j
	int *mpMaterialIndex;//�}�e���A���ԍ�
	std::vector<CMaterial*> mMaterial;//�}�e���A���f�[�^
	std::vector<CSkinWeights*> mSkinWeights;
	CVector3 *mpAnimateVertex; //�A�j���[�V�����p���_
	CVector3 *mpAnimateNormal; //�A�j���[�V�����p�@��
	//�e�N�X�`�����W�f�[�^
	CVector2 *mpTextureCoords;
	//�R���X�g���N�^
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
	//�f�X�g���N�^
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		SAFE_DELETE_ARRAY(mpAnimateVertex);
		SAFE_DELETE_ARRAY(mpAnimateNormal);
		SAFE_DELETE_ARRAY(mpTextureCoords);
		//�X�L���E�F�C�g�̍폜
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	//�ǂݍ��ݏ���
	void Init(CModel *model);
	void Render();
	//���_�ɃA�j���[�V�����K�p
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
	//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CAnimationSet*>mAnimationSet;

	std::vector<CMaterial*> mMaterial; //�}�e���A���̔z��

	std::vector<CTexture*> mTexture; //�e�N�X�`���̔z��
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();

	//�}�e���A���̌���
	CMaterial* FindMaterial(char* name);

	//�e�N�X�`���̌���
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
	�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	*/
	CModelXFrame* CModel::FindFrame(char* name){
		//�C�e���[�^�̍쐬
		std::vector<CModelXFrame*>::iterator itr;
		//�擪����Ō�܂ŌJ��Ԃ�
		for (itr = mFrame.begin(); itr != mFrame.end(); itr++){
			//���O����v�������H
			if (strcmp(name, (*itr)->mpName) == 0){
				//��v�����炻�̃A�h���X��Ԃ�
				return *itr;
			}
		}
		//��v����t���[���������ꍇ��NULL��Ԃ�
		return NULL;
	}
};


#endif
