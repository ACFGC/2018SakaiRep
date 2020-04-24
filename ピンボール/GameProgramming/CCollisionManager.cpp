#include"CCollisionManager.h"
#include"CCollider.h"
CCollisionManager* CCollisionManager::mpInstance = 0;
CCollisionManager* CCollisionManager::Get(){
	if (mpInstance == 0){
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}
void CCollisionManager::Add(CTask *task){
	if (mpHead){
		//2���ڈȍ~�̏���
		//������c�ɐ擪��������
		CTask *c = mpHead;
		//�擪�ɒǉ��̏ꍇ
		if (task->mPriority >= c->mPriority){
			//�擪�͈����̃^�X�N
			mpHead = task;
			//�擪�̑O�͂Ȃ�
			task->mpPrev = 0;
			//�擪�̎��́A�J�����gc
			task->mpNext = c;
			//���̑O�͈����̃^�X�N
			c->mpPrev = task;
			return;
		}//�擪�ɒǉ��̏I���
		c = c->mpNext;
		//�擪�ȍ~�ɒǉ�
		while (c){
			if (task->mPriority >= c->mPriority){
				//�J�����g�̑O�̎��������̃^�X�N�ɂ���
				c->mpPrev->mpNext = task;
				//�����̃^�X�N�̑O���J�����g�̑O��
				task->mpPrev = c->mpPrev;
				//�����̃^�X�N�̎����J�����g��
				task->mpNext = c;
				//�J�����g�̑O�������̃^�X�N��
				c->mpPrev = task;
				return;
			}
			c = c->mpNext;
		}//�r���ɒǉ��I��
		//�Ō�ɒǉ�
		//�Ō�̎��������̃^�X�N�ɂ���
		mpTail->mpNext = task;
		//�����̃^�X�N�̑O���A���̍Ō�ɂ���
		task->mpPrev = mpTail;
		//�����̃^�X�N�̎���0�ɂ���
		task->mpNext = 0;
		//�Ō�Ɉ����̃^�X�N��������
		mpTail = task;
	}
	else{//0�͋U
		//1���ڂ̏���
		//�擪�������̃^�X�N�ɂ���
		mpHead = task;
		//�擪�̑O��0�ɂ���
		task->mpPrev = 0;
		//�擪�̎���0�ɂ���
		task->mpNext = 0;
		//�Ō�Ɉ����̃^�X�N��������
		mpTail = task;
	}
}
void CCollisionManager::Update(){
	//�J�����gc��擪�ɂ���
	CCollider *c = (CCollider*)mpHead;
	//�J�����gc��0�ɂȂ�����I��
	while (c){
		//������Փ˔���
		CCollider *y = (CCollider*)c->mpNext;
		while (y){
			//�����e�̃R���C�_�̓X�L�b�v
			//if (y->mpParent != c->mpParent){
			//	//�e�̏Փ˔�����Ă�
			//	if (c->mpParent)
			//		c->mpParent->Collision(c, y);
			//	if (y->mpParent)
			//		y->mpParent->Collision(y, c);
			//}
			//���̑����
			y = (CCollider*)y->mpNext;
		}
#ifdef _DEBUG
		//�J�����gc��`��
		c->Render();
#endif
		c = (CCollider*)c->mpNext;
	}
}
void CCollisionManager::Remove(CTask *task) {
	if (mpHead == task) {
		if (mpTail == task) {
			mpHead = mpTail = 0;
		}
		else {
			mpHead = task->mpNext;
			mpHead->mpPrev = 0;
		}
		return;
	}
	if (mpTail == task) {
		mpTail = task->mpPrev;
		mpTail->mpNext = 0;
		return;
	}
	task->mpPrev->mpNext = task->mpNext;
	task->mpNext->mpPrev = task->mpPrev;
}