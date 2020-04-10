#ifndef CCOLLISIONMANAGER
#define CCOLLISIONMANAGER
#include "CTask.h"
class CCollisionManager{
	static CCollisionManager *mpInstance;
	CTask *mpHead;
	CTask *mpTail;
public:
	CCollisionManager()
		: mpHead(0)
		, mpTail(0)
	{}
	static CCollisionManager* Get();
	void Add(CTask *p);
	//void Add(Collider *p);
	//void Collision(CBoxCollider *p);
	//void Collision(CCircleCollider *p);
	//void Collision(CSphereCollider *p);
	void Destroy();
	void Remove(CTask *task);
	void Update();
};
#endif