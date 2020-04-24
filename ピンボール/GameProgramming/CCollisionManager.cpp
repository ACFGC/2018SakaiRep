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
		//2件目以降の処理
		//かれんとcに先頭を代入する
		CTask *c = mpHead;
		//先頭に追加の場合
		if (task->mPriority >= c->mPriority){
			//先頭は引数のタスク
			mpHead = task;
			//先頭の前はなし
			task->mpPrev = 0;
			//先頭の次は、カレントc
			task->mpNext = c;
			//次の前は引数のタスク
			c->mpPrev = task;
			return;
		}//先頭に追加の終わり
		c = c->mpNext;
		//先頭以降に追加
		while (c){
			if (task->mPriority >= c->mPriority){
				//カレントの前の次を引数のタスクにする
				c->mpPrev->mpNext = task;
				//引数のタスクの前をカレントの前に
				task->mpPrev = c->mpPrev;
				//引数のタスクの次をカレントに
				task->mpNext = c;
				//カレントの前を引数のタスクに
				c->mpPrev = task;
				return;
			}
			c = c->mpNext;
		}//途中に追加終了
		//最後に追加
		//最後の次を因数のタスクにする
		mpTail->mpNext = task;
		//引数のタスクの前を、今の最後にする
		task->mpPrev = mpTail;
		//引数のタスクの次を0にする
		task->mpNext = 0;
		//最後に引数のタスクを代入する
		mpTail = task;
	}
	else{//0は偽
		//1件目の処理
		//先頭を引数のタスクにする
		mpHead = task;
		//先頭の前を0にする
		task->mpPrev = 0;
		//先頭の次を0にする
		task->mpNext = 0;
		//最後に引数のタスクを代入する
		mpTail = task;
	}
}
void CCollisionManager::Update(){
	//カレントcを先頭にする
	CCollider *c = (CCollider*)mpHead;
	//カレントcが0になったら終了
	while (c){
		//次から衝突判定
		CCollider *y = (CCollider*)c->mpNext;
		while (y){
			//同じ親のコライダはスキップ
			//if (y->mpParent != c->mpParent){
			//	//親の衝突判定を呼ぶ
			//	if (c->mpParent)
			//		c->mpParent->Collision(c, y);
			//	if (y->mpParent)
			//		y->mpParent->Collision(y, c);
			//}
			//次の相手へ
			y = (CCollider*)y->mpNext;
		}
#ifdef _DEBUG
		//カレントcを描画
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