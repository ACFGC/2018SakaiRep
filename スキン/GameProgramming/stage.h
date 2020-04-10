#ifndef STAGE
#define STAGE
#include "CCharacter.h"
#include "CCollider.h"

class stage : public CCharacter {
public:
	CCollider mCollider;

	stage(CModel *model, float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

	void Update();
	/*void OnCollision(CCollider *m, CCollider *y);*/
};
#endif