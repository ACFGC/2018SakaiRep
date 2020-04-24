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
	CModel *mpModel; //���f���f�[�^
	int mAnimationIndex; //�A�j���[�V�����ԍ�
	bool mAnimationLoopFlg; //ture:�J��Ԃ�
	float mAnimationFrame; //�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;//�A�j���[�V�����̍Đ��t���[����
	//����������
	void Init(CModel *model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�Փˏ���
	virtual bool Collision(CCollider*, CCollider*){
		return false;
	};
	//�L�����N�^�[�̎���
	enum ECharTag{
		EPLAYER = 1,//1���珇��
		EENEMY,
	};
	ECharTag mTag; //�L�����N�^���ʃ^�O
	CVector3 mPosition2; //�ʒu
	CVector3 mRotation2; //��]�l
	CMatrix44 mMatrix44; //�ʒu��]�s��
	//�X�V����
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
