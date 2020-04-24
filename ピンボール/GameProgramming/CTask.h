#ifndef CTASK
#define CTASK
#include"CMatrix44.h"
class CTask {
public:
	enum ETaskTag{
		EMATERIAL,
		EENEMYTANK,
		EPLAYERBULLET,
		EENEMYBULLET,
		EGROUND,
		EGOAL,
		ECOLSPHERE,
	};
	ETaskTag mTaskTag;
	CTask *mpNext;
	CTask *mpPrev;
	int mPriority;
	bool mEnabled;
	CMatrix44 mMatrix;
	CTask()
		: mpNext(0)
		, mpPrev(0)
		, mPriority(0)
		, mEnabled(true) {}
	virtual ~CTask() {};
	virtual void Update() {};
	virtual void Render() {};
};
class CTaskManager{
	protected:
		CTask *mpHead;
		CTask *mpTail;
	public:
		static CTaskManager* Get();
		CTaskManager();
		void Add(CTask *task);
		void Update();
		void Render();
		void Delete();
		void Remove(CTask *task);
		void Destory();
	
};


#endif
